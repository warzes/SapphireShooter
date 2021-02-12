#pragma once

#include "Framebuffer2.h"
#include "MultisampledFrameBuffer.h"
#include "MultisampledRenderBuffer.h"
#include "ShaderProgram.h"

class PostProcessing
{
	FrameBuffer buffer;
	RenderBuffer renderBuffer;
	MultisampledRenderBuffer mulitsampledRenderBuffer;
	MultisampledFrameBuffer multisampledBuffer;
	unsigned attachments;

public:
	PostProcessing(const unsigned& attach = 1);
	virtual ~PostProcessing() { ; }

	void startProcessing() const;
	void endProcessing() const;
	void updateBuffers();
	const std::vector<Texture>& getResultTextures() const { return buffer.getTextures(); }

	static void renderToQuad(std::shared_ptr<ShaderProgram> program, const std::vector<unsigned>& textures, const glm::mat4& model = glm::mat4(1.0f));
	static void renderToQuad(std::shared_ptr<ShaderProgram> program, const std::vector<Texture>& textures, const glm::mat4& model = glm::mat4(1.0f));
};