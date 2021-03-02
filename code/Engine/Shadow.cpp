#include "stdafx.h"
#include "Shadow.h"

Shadow::Shadow()
{
	depth.setDepthBuffer(depthBuffer);
	depth.generate(shadowSize, shadowSize, 0);
}

void Shadow::updateBuffer()
{
	depth.removeBuffers();
	depth.generate(shadowSize, shadowSize, 0);
}

void Shadow::initCastShadow(const Light& light, const std::vector<std::shared_ptr<ShaderProgram>>& programs)
{
	for (unsigned i = 0; i < programs.size(); ++i)
	{
		programs[i]->Bind();
		programs[i]->SetFloat("farPlane", shadowFar);
		programs[i]->SetVector3f("lightPos", light.getPosition());
	}

	const ShadowTransforms& shadowTransforms = light.getShadowTransforms();
	for (unsigned i = 0; i < shadowTransforms.size(); ++i)
	{
		const char* name = std::string("shadowMatrices[" + std::to_string(i) + "]").c_str();
		for (unsigned j = 0; j < programs.size(); ++j)
		{
			programs[j]->Bind();
			programs[j]->SetMatrix4(name, shadowTransforms[i]);
		}
	}
}

void Shadow::startCastShadow(const Light& light, const std::vector<std::shared_ptr<ShaderProgram>>& programs)
{
	initCastShadow(light, programs);
	glViewport(0, 0, shadowSize, shadowSize);
	depth.bind();
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Shadow::endCastShadow(const Light& light, const std::vector<std::shared_ptr<ShaderProgram>>& programs)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	const char* name = std::string("shadowMaps[" + std::to_string(light.getIndex()) + "]").c_str();
	for (unsigned i = 0; i < programs.size(); ++i)
	{
		programs[i]->Bind();
		programs[i]->SetInteger(name, 9 + light.getIndex());
		programs[i]->SetFloat("farPlane", shadowFar);
		Texture::active(9 + light.getIndex());
		depthBuffer.bind(GL_TEXTURE_CUBE_MAP);
	}
}