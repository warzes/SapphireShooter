#include "stdafx.h"
#include "Terrain2.h"

Terrain2::Terrain2()
{
	amount = new unsigned;
	*amount = 1;
}

Terrain2::Terrain2(const Terrain2& terrain)
{
	swap(terrain);
}

void Terrain2::init()
{
	glGenVertexArrays(1, &VAO);
}

Terrain2& Terrain2::operator=(const Terrain2& terrain)
{
	swap(terrain);
	return *this;
}

void Terrain2::swap(const Terrain2& terrain)
{
	textureStone = terrain.textureStone;
	textureGrass = terrain.textureGrass;
	textureHeight = terrain.textureHeight;
	textureNormal = terrain.textureNormal;
	depth = terrain.depth;
	maxTessLevel = terrain.maxTessLevel;
	VAO = terrain.VAO;
	amount = terrain.amount;
	*amount = *amount + 1;
}

Terrain2::~Terrain2()
{
	clear();
}

void Terrain2::clear()
{
	*amount = *amount - 1;
	if (*amount == 0)
	{
		delete amount;
		glDeleteVertexArrays(1, &VAO);
	}
}

void Terrain2::render(std::shared_ptr<ShaderProgram> program)
{
	//glDisable(GL_CULL_FACE);

	//Transformationable::render(program);
	//program.SetFloat("depth", depth);
	//program.SetInteger("maxTessLevel", maxTessLevel);

	//Texture::active(0);
	//program.SetInteger("heightMap", 0);
	//textureHeight.bind(GL_TEXTURE_2D);

	//Texture::active(1);
	//program.SetInteger("normalMap", 1);
	//textureNormal.bind(GL_TEXTURE_2D);

	//Texture::active(2);
	//program.SetInteger("stoneTex", 2);
	//textureStone.bind(GL_TEXTURE_2D);

	//Texture::active(3);
	//program.SetInteger("grassTex", 3);
	//textureGrass.bind(GL_TEXTURE_2D);

	//glBindVertexArray(VAO);
	//glPatchParameteri(GL_PATCH_VERTICES, 4);
	//glDrawArraysInstanced(GL_PATCHES, 0, 4, 64 * 64);
	//glBindVertexArray(0);
	//Texture::unbind(GL_TEXTURE_2D);
	//glEnable(GL_CULL_FACE);
}