#include "stdafx.h"
#include "Water.h"
#include "Engine.h"

void Water::render(std::shared_ptr<ShaderProgram> program)
{
	offset += 0.03 * Engine::Get().GetDeltaTime();
	if (offset >= 100)
		offset = 0;
	program->SetFloat("waterOffset", offset);

	Texture::active(0);
	program->SetInteger("refractionTex", 0);
	glBindTexture(GL_TEXTURE_2D, buffers.getRefractTextureId());

	Texture::active(1);
	program->SetInteger("reflectionTex", 1);
	glBindTexture(GL_TEXTURE_2D, buffers.getReflectTextureId());

	Texture::active(2);
	program->SetInteger("dudvMap", 2);
	dudvMap.bind(GL_TEXTURE_2D);

	Texture::active(3);
	program->SetInteger("normalMap", 3);
	normalMap.bind(GL_TEXTURE_2D);

	Texture::active(4);
	program->SetInteger("depthMap", 4);
	glBindTexture(GL_TEXTURE_2D, buffers.getRefractDepthTextureId());

	program->SetFloat("nearPlane", cameraNear);
	program->SetFloat("farPlane", cameraFar);
	Transformationable::render(program);
	plane.render(program);
	Texture::unbind(GL_TEXTURE_2D);
}

void Water::renderReflectAndRefract(Scene* scene)
{
	glm::vec3 defaultCamPos = camera->GetCameraPos();
	float defaultCamPitch = camera->GetPitch();

	float distance = 2 * (defaultCamPos.y - getPosition().y);
	camera->SetCameraPos(glm::vec3(defaultCamPos.x, defaultCamPos.y - distance, defaultCamPos.z));
	camera->SetPitch(-defaultCamPitch);
	camera->updateVectors();

	buffers.bindReflectBuffer();
	scene->render(glm::vec4(0, 1, 0, -getPosition().y + 0.1));
	FrameBuffer::unbind();

	camera->SetCameraPos(defaultCamPos);
	camera->SetPitch(defaultCamPitch);
	camera->updateVectors();

	buffers.bindRefractBuffer();
	scene->render(glm::vec4(0, -1, 0, getPosition().y));
	FrameBuffer::unbind();
}