#pragma once

#include "Texture.h"
#include "Shape2.h"

class Skybox : public Shape2
{
	Texture texture;


public:
	Skybox() { /*init();*/ }
	Skybox(const Texture& tex) :texture(tex) { /*init();*/ }

	void init();

	virtual void render(Program& program);

	Texture& getTexture() { return texture; }
};