#include "stdafx.h"
#include "Water.h"
#include "Engine.h"
#include "Scene.h"
#include "ShaderManager.h"
//-----------------------------------------------------------------------------
void Water::Create()
{
	m_dudvMap.load("textures/water-dudv.jpg", GL_TEXTURE_2D);
	m_normalMap.load("textures/water-normal.jpg", GL_TEXTURE_2D);
}
//-----------------------------------------------------------------------------
void Water::Render(std::shared_ptr<ShaderProgram> program)
{
	m_offset += 0.03 * Engine::Get().GetDeltaTime();
	if (m_offset >= 100)
		m_offset = 0;
	program->SetFloat("waterOffset", m_offset);

	Texture::active(0);
	program->SetInteger("refractionTex", 0);
	glBindTexture(GL_TEXTURE_2D, buffers.getRefractTextureId());

	Texture::active(1);
	program->SetInteger("reflectionTex", 1);
	glBindTexture(GL_TEXTURE_2D, buffers.getReflectTextureId());

	Texture::active(2);
	program->SetInteger("dudvMap", 2);
	m_dudvMap.bind(GL_TEXTURE_2D);

	Texture::active(3);
	program->SetInteger("normalMap", 3);
	m_normalMap.bind(GL_TEXTURE_2D);

	Texture::active(4);
	program->SetInteger("depthMap", 4);
	glBindTexture(GL_TEXTURE_2D, buffers.getRefractDepthTextureId());

	program->SetFloat("nearPlane", cameraNear);
	program->SetFloat("farPlane", cameraFar);
	Transformationable::Render(program);
	m_plane.Render(program);
	Texture::unbind(GL_TEXTURE_2D);
}
//-----------------------------------------------------------------------------
void Water::RenderReflectAndRefract(Scene2* scene)
{
	glm::vec3 defaultCamPos = m_camera->GetCameraPos();
	float defaultCamPitch = m_camera->GetPitch();

	float distance = 2 * (defaultCamPos.y - GetPosition().y);
	m_camera->SetCameraPos(glm::vec3(defaultCamPos.x, defaultCamPos.y - distance, defaultCamPos.z));
	m_camera->SetPitch(-defaultCamPitch);
	m_camera->Update(); // TODO: подумать

	buffers.bindReflectBuffer();
	scene->render(glm::vec4(0, 1, 0, -GetPosition().y + 0.1));
	FrameBuffer::unbind();

	m_camera->SetCameraPos(defaultCamPos);
	m_camera->SetPitch(defaultCamPitch);
	m_camera->Update(); // TODO: подумать

	buffers.bindRefractBuffer();
	scene->render(glm::vec4(0, -1, 0, GetPosition().y));
	FrameBuffer::unbind();
}
//-----------------------------------------------------------------------------