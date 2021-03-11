#include "stdafx.h"
#if SE_OPENGL
#include "Terrain.h"
#include "ShaderManager.h"
#include "OGLFunc.h"
#include "TextureManager.h"
//-----------------------------------------------------------------------------
Terrain::Terrain() :
	m_fTerrainHeight(70.0f), m_cellSpacing(3.0f),
	m_terrainLength(256), m_terrainWidth(256),
	m_model(1.0f),
	m_terrainXPos(0.0f),
	m_terrainZPos(0.0f)
{
	m_model = glm::translate(glm::vec3(m_terrainXPos, 0.0f, m_terrainZPos));
}
//-----------------------------------------------------------------------------
void Terrain::SetPos(float x, float y)
{
	m_model = glm::translate(glm::vec3(x, 0.0f, y));
}
//-----------------------------------------------------------------------------
void Terrain::InitTerrain()
{
	m_terrainShader = ShaderManager::Get().LoadShader("terrain", "../res/Shaders/TerrainVertexShader.vs", "../res/Shaders/TerrainFragmentShader.fs");

	m_textures.resize(6);
	m_textures[0] = TextureManager::Get().LoadTexture("soil", "../res/Textures/glass_lime.png", false);
	m_textures[1] = TextureManager::Get().LoadTexture("soil2", "../res/Textures/glass_orange.png", false);
	m_textures[2] = TextureManager::Get().LoadTexture("grass", "../res/Textures/grass.png", false);
	m_textures[3] = TextureManager::Get().LoadTexture("soil4", "../res/Textures/glass_gray.png", false);
	m_textures[4] = TextureManager::Get().LoadTexture("blendMap", "../res/Textures/blendMap.png", false);
	m_textures[5] = TextureManager::Get().LoadTexture("grassNormalMap", "../res/Textures/soil03_NormalMap.jpg", false);
}
//-----------------------------------------------------------------------------
// Function that creates a heightmap (procedurally) using the Perlin Noise algorithm
void Terrain::CreateTerrainWithPerlinNoise()
{
	// Set the seed of the noise so that it is different every time
	noise.SetSeed(static_cast<unsigned int>(time(0)));
	double frequency = 4.2;
	int octaves = 5;

	const double fx = 256.0 / frequency;
	const double fy = 256.0 / frequency;

	std::vector<float> tmp;
	for (unsigned int y = 0; y < 256; ++y)
	{
		for (unsigned int x = 0; x < 256; ++x)
		{
			const double colorR = noise.OctaveNoise(x / fx, y / fy, octaves);
			tmp.push_back((float)colorR);
		}

		m_vHeights.push_back(tmp);
		tmp.clear();
	}

	std::vector<glm::vec3> Vertices;
	std::vector<glm::vec2> Textures;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec3> tangents;

	// Calculate vertices
	int terrainHeightOffsetFront = 50;
	int terrainHeightOffsetBack = 0;
	int terrainHeightOffsetLeftSide = 50;
	int terrainHeightOffsetRightSide = 0;

	for (unsigned int i = 0; i < m_vHeights.size(); ++i)
	{
		terrainHeightOffsetFront = 50;
		terrainHeightOffsetBack = 0;
		terrainHeightOffsetLeftSide -= 4;

		if (i > 245)
			terrainHeightOffsetRightSide += 4;

		for (unsigned int j = 0; j < m_vHeights[0].size(); ++j)
		{
			// бордюр
			// TODO: можно сделать в обратную сторону (вниз) чтобы получить лет остров
			/*if (i < 12)
			{
				Vertices.push_back(glm::vec3(i * m_cellSpacing, m_vHeights[i][j] * m_fTerrainHeight + terrainHeightOffsetLeftSide, j * m_cellSpacing));
			}
			else if (i > 245)
			{
				Vertices.push_back(glm::vec3(i * m_cellSpacing, m_vHeights[i][j] * m_fTerrainHeight + terrainHeightOffsetRightSide, j * m_cellSpacing));
			}
			else if (j < 12)
			{
				Vertices.push_back(glm::vec3(i * m_cellSpacing, m_vHeights[i][j] * m_fTerrainHeight + terrainHeightOffsetFront, j * m_cellSpacing));
				terrainHeightOffsetFront -= 4;
			}
			else if (j > 245)
			{
				terrainHeightOffsetBack += 4;

				if (terrainHeightOffsetBack > 70)
					terrainHeightOffsetBack = 70;

				Vertices.push_back(glm::vec3(i * m_cellSpacing, m_vHeights[i][j] * m_fTerrainHeight + terrainHeightOffsetBack, j * m_cellSpacing));
			}
			else*/
			{
				terrainHeightOffsetFront = 50;
				Vertices.push_back(glm::vec3(i * m_cellSpacing, m_vHeights[i][j] * m_fTerrainHeight, j * m_cellSpacing));
			}
			Textures.push_back(glm::vec2(i * 1.0f / m_vHeights.size(), j * 1.0f / m_vHeights[0].size()));
			Normals.push_back(CalculateNormal(i, j));
		}
	}

	// Calculate indices
	for (unsigned int i = 0; i < m_vHeights.size() - 1; ++i)
	{
		for (unsigned int j = 0; j < m_vHeights.size() - 1; ++j)
		{
			m_indices.push_back((i * m_vHeights[0].size()) + j);
			m_indices.push_back((i * m_vHeights[0].size()) + j + 1);
			m_indices.push_back(((i + 1) * m_vHeights[0].size()) + j);

			m_indices.push_back((i * m_vHeights[0].size()) + j + 1);
			m_indices.push_back(((i + 1) * m_vHeights[0].size()) + j);
			m_indices.push_back(((i + 1) * m_vHeights[0].size()) + j + 1);
		}
	}

	// Calculate tangents
	for (unsigned int i = 0; i < m_vHeights.size(); ++i)
	{
		for (unsigned int j = 0; j < m_vHeights[0].size(); ++j)
		{
			int vertexIndex = j + i * 256;
			glm::vec3 v1 = Vertices[vertexIndex];

			if (j < 255)
			{
				glm::vec3 v2 = Vertices[vertexIndex + 1];
				glm::vec3 result = v1 - v2;
				result = glm::normalize(result);
				tangents.push_back(result);
			}
			else
			{
				glm::vec3 v2 = Vertices[vertexIndex - 1];
				glm::vec3 result = v1 - v2;
				result = glm::normalize(result);
				tangents.push_back(result);
			}
		}
	}

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(TOTAL_BUFFERS, m_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec3), &Vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[TEXTURE_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, Textures.size() * sizeof(glm::vec2), &Textures[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[NORMAL_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(glm::vec3), &Normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[TANGENT_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glGenBuffers(1, &m_VBO[ELEMENT_BUFFER]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[ELEMENT_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
//-----------------------------------------------------------------------------
// Function that calculate the normals for the terrain
glm::vec3 Terrain::CalculateNormal(unsigned int x, unsigned int z)
{
	if (x >= 0 && x < m_terrainWidth - 1 && z >= 0 && z < m_terrainLength - 1)
	{
		float heightL = GetHeightOfTerrain((float)x - 1, (float)z);
		float heightR = GetHeightOfTerrain((float)x + 1, (float)z);
		float heightD = GetHeightOfTerrain((float)x, (float)z - 1);
		float heightU = GetHeightOfTerrain((float)x, (float)z + 1);

		glm::vec3 normal(heightL - heightR, 2.0f, heightD - heightU);
		normal = glm::normalize(normal);
		return normal;
	}

	return glm::vec3(0.0f, 0.0f, 0.0f);
}
//-----------------------------------------------------------------------------
// Function that gets the heights of the terrain grids
float Terrain::GetHeightOfTerrain(float _X, float _Z)
{
	float result = 0.0f;

	float terrainX = _X - m_terrainXPos;
	float terrainZ = _Z - m_terrainZPos;

	// calculate length of grid square
	float gridSquareLength = m_terrainLength * m_cellSpacing / ((float)m_terrainWidth - 1);

	// Check which grid square the player is in
	int gridX = (int)std::floor(terrainX / gridSquareLength);
	int gridZ = (int)std::floor(terrainZ / gridSquareLength);

	// Check if position is on the terrain
	if (gridX >= m_terrainWidth - 1 || gridZ >= m_terrainLength - 1 || gridX < 0 || gridZ < 0)
	{
		return 0.0f;
	}

	// Find out where the player's position is on the terrain
	float xCoord = std::fmod(terrainX, gridSquareLength) / gridSquareLength;
	float zCoord = std::fmod(terrainZ, gridSquareLength) / gridSquareLength;

	// Top triangle of the quad, else the bottom triangle of the quad
	if (xCoord <= (1 - zCoord))
	{
		result = BarryCentric(glm::vec3(0, m_vHeights[gridX][gridZ] * m_fTerrainHeight, 0),
			glm::vec3(1, m_vHeights[gridX + 1][gridZ] * m_fTerrainHeight, 0),
			glm::vec3(0, m_vHeights[gridX][gridZ + 1] * m_fTerrainHeight, 1),
			glm::vec2(xCoord, zCoord));
	}
	else
	{
		result = BarryCentric(glm::vec3(1, m_vHeights[gridX + 1][gridZ] * m_fTerrainHeight, 0),
			glm::vec3(1, m_vHeights[gridX + 1][gridZ + 1] * m_fTerrainHeight, 1),
			glm::vec3(0, m_vHeights[gridX][gridZ + 1] * m_fTerrainHeight, 1),
			glm::vec2(xCoord, zCoord));
	}

	return result;
}
//-----------------------------------------------------------------------------
// Helper function used to find the height of a triangle the player is currently on
float Terrain::BarryCentric(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const glm::vec2 &pos)
{
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}
//-----------------------------------------------------------------------------
// Function that binds the terrain vertex array object (VAO) and draws its vertex data
void Terrain::Draw(Camera3D& _cam, DirectionalLight* directionLight, PointLight* lamp, SpotLight* spotlight)
{
	m_terrainShader->Bind();

	// Texture properties
	m_terrainShader->SetInteger("meshTexture", 0);
	m_terrainShader->SetInteger("rTexture", 1);
	m_terrainShader->SetInteger("gTexture", 2);
	m_terrainShader->SetInteger("bTexture", 3);
	m_terrainShader->SetInteger("blendMap", 4);
	m_terrainShader->SetInteger("grassNormalMap", 5);

	// Activate all the textures
	for (unsigned int i = 0; i < 6; ++i)
		m_textures[i]->Bind(i);

	// MVP transformation matrix
	m_terrainShader->SetMatrix4("model", m_model);
	m_terrainShader->SetMatrix4("view", _cam.GetViewMatrix());
	m_terrainShader->SetMatrix4("projection", _cam.GetProjectionMatrix());
	m_terrainShader->SetVector3f("viewPos", _cam.GetCameraPos());

	// ----------------------
	// Lighting properties 
	// ----------------------

	// a. Directional Light
	m_terrainShader->SetVector3f("dirLight.direction", directionLight->GetDirection());
	m_terrainShader->SetVector3f("dirLight.ambient", directionLight->GetAmbient());
	m_terrainShader->SetVector3f("dirLight.diffuse", directionLight->GetDiffuse());
	m_terrainShader->SetVector3f("dirLight.specular", directionLight->GetSpecular());
	m_terrainShader->SetVector3f("dirLight.lightColour", directionLight->GetColour());

	// b. Some light bulbs
	m_terrainShader->SetVector3f("pointLight.position", lamp->GetPos());
	m_terrainShader->SetVector3f("pointLight.ambient", lamp->GetAmbient());
	m_terrainShader->SetVector3f("pointLight.diffuse", lamp->GetDiffuse());
	m_terrainShader->SetVector3f("pointLight.specular", lamp->GetSpecular());
	m_terrainShader->SetVector3f("pointLight.lightColour", lamp->GetColour());
	m_terrainShader->SetFloat("pointLight.constant", lamp->GetConstant());
	m_terrainShader->SetFloat("pointLight.linear", lamp->GetLinear());
	m_terrainShader->SetFloat("pointLight.quadratic", lamp->GetQuadratic());

	// c. Spotlight
	m_terrainShader->SetVector3f("spotlight.position", spotlight->GetPosition());
	m_terrainShader->SetVector3f("spotlight.direction", spotlight->GetDirection());
	m_terrainShader->SetVector3f("spotlight.diffuse", spotlight->GetDiffuse());
	m_terrainShader->SetVector3f("spotlight.specular", spotlight->GetSpecular());
	m_terrainShader->SetFloat("spotlight.constant", spotlight->GetConstant());
	m_terrainShader->SetFloat("spotlight.linear", spotlight->GetLinear());
	m_terrainShader->SetFloat("spotlight.quadratic", spotlight->GetQuadratic());
	m_terrainShader->SetFloat("spotlight.cutOff", glm::cos(glm::radians(spotlight->GetCutOff())));
	m_terrainShader->SetFloat("spotlight.outerCutOff", glm::cos(glm::radians(spotlight->GetOuterCutOff())));

	// Fog effect
	if (m_fog)
		m_terrainShader->SetBool("fogActive", true);
	else
		m_terrainShader->SetBool("fogActive", false);

	// Draw the terrain
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[ELEMENT_BUFFER]);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_terrainShader->UnBind();
}
//-----------------------------------------------------------------------------
#endif