#pragma once

#include "Camera3D.h"
#include "PerlinNoise.h"
#include "ShaderProgram.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Texture2D.h"

class Terrain
{
public:
	Terrain();

	void SetPos(float x, float y);

	float GetHeightOfTerrain(float x, float y);
	float BarryCentric(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const glm::vec2 &pos);
	void InitTerrain();
	void CreateTerrainWithPerlinNoise();
	glm::vec3 CalculateNormal(unsigned int x, unsigned int z);
	void SetFog(bool fogState) { m_fog = fogState; }

	void Draw(Camera3D& cam, DirectionalLight* directionLight, PointLight* lamp, SpotLight* spotlight);

private:
	std::shared_ptr<ShaderProgram> m_terrainShader;
	glm::mat4 m_model;

	std::vector<std::shared_ptr<Texture2D>> m_textures;

	enum { VERTEX_BUFFER, TEXTURE_BUFFER, NORMAL_BUFFER, TANGENT_BUFFER, ELEMENT_BUFFER, TOTAL_BUFFERS };

	GLuint m_VAO;
	GLuint m_VBO[TOTAL_BUFFERS];

	float m_cellSpacing, m_fTerrainHeight;
	float m_terrainLength;
	float m_terrainWidth;
	float m_terrainXPos;
	float m_terrainZPos;
	bool m_fog;

	std::vector<std::vector<float> > m_vHeights;
	std::vector<unsigned int> m_indices;

	std::uint32_t seed;
	PerlinNoise noise;
};