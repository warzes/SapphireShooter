#pragma once

#include "Texture.h"
#include "Transformationable.h"

class Terrain2 : public Transformationable
{
	Texture textureGrass;
	Texture textureStone;
	Texture textureHeight;
	Texture textureNormal;
	float depth = 0.05;
	unsigned maxTessLevel = 0;
	unsigned VAO = 0;
	unsigned* amount;

	void swap(const Terrain2& terrain);
	void clear();

public:
	Terrain2();
	Terrain2(const Terrain2& terrain);
	Terrain2& operator=(const Terrain2& terrain);
	virtual ~Terrain2();

	void init();

	virtual void render(std::shared_ptr<ShaderProgram> program);
	virtual unsigned getVAO() const { return VAO; }

	Texture& getHeightMap() { return textureHeight; }
	Texture& getNormalMap() { return textureNormal; }
	Texture& getStoneTexture() { return textureStone; }
	Texture& getGrassTexture() { return textureGrass; }

	void setDepth(const float& d) { depth = d; }
	float getDepth() const { return depth; }

	void setTessLevel(const unsigned& tl) { maxTessLevel = tl; }
	unsigned getTessLevel() const { return maxTessLevel; }
};