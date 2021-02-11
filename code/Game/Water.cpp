#include "stdafx.h"
#include "Water.h"
#include "Engine.h"

void Water::render(Program& program)
{
	offset += 0.03 * Engine::Get().GetDeltaTime();
	if (offset >= 100)
		offset = 0;
	program.SetFloat("waterOffset", offset);

	Texture::active(0);
	program.SetInt("refractionTex", 0);
	glBindTexture(GL_TEXTURE_2D, buffers.getRefractTextureId());

	Texture::active(1);
	program.SetInt("reflectionTex", 1);
	glBindTexture(GL_TEXTURE_2D, buffers.getReflectTextureId());

	Texture::active(2);
	program.SetInt("dudvMap", 2);
	dudvMap.bind(GL_TEXTURE_2D);

	Texture::active(3);
	program.SetInt("normalMap", 3);
	normalMap.bind(GL_TEXTURE_2D);

	Texture::active(4);
	program.SetInt("depthMap", 4);
	glBindTexture(GL_TEXTURE_2D, buffers.getRefractDepthTextureId());

	program.SetFloat("nearPlane", cameraNear);
	program.SetFloat("farPlane", cameraFar);
	Transformationable::render(program);
	plane.render(program);
	Texture::unbind(GL_TEXTURE_2D);
}

void Water::renderReflectAndRefract(Scene* scene)
{
	glm::vec3 defaultCamPos = camera->getPosition();
	float defaultCamPitch = camera->getPitch();

	float distance = 2 * (defaultCamPos.y - getPosition().y);
	camera->setPosition(glm::vec3(defaultCamPos.x, defaultCamPos.y - distance, defaultCamPos.z));
	camera->setPitch(-defaultCamPitch);
	camera->updateVectors();

	buffers.bindReflectBuffer();
	scene->render(glm::vec4(0, 1, 0, -getPosition().y + 0.1));
	FrameBuffer::unbind();

	camera->setPosition(defaultCamPos);
	camera->setPitch(defaultCamPitch);
	camera->updateVectors();

	buffers.bindRefractBuffer();
	scene->render(glm::vec4(0, -1, 0, getPosition().y));
	FrameBuffer::unbind();
}