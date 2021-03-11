#pragma once

#if SE_OPENGL

#include "Texture.h"
#include "Geometry.h"

class Skybox : public Geometry2
{
	Texture texture;


public:
	Skybox() { /*init();*/ }
	Skybox(const Texture& tex) :texture(tex) { /*init();*/ }

	void init();

	virtual void Render(std::shared_ptr<ShaderProgram> program);

	Texture& getTexture() { return texture; }
};

#endif