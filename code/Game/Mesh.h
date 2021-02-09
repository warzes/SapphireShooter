#pragma once

#include "Transformation.h"
#include "Camera.h"
#include "Texture2D.h"
#include "ShaderProgram.h"

struct MeshVertex
{
public:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct MeshTexture
{
public:
	std::shared_ptr<Texture2D> tex;
	std::string type;
	aiString path;
};

class Mesh
{
public:
	Mesh(
		const std::vector<MeshVertex>& vertices,
		const std::vector<GLuint>& indices,
		const std::vector<MeshTexture>& textures,
		bool instancing);

	glm::mat4* GetModelMatIns() { return m_modelMatricesIns; }

	void SetTransform(Transform& transform) { m_transform = transform; }

	void Draw(
		Camera& camera,
		std::shared_ptr<ShaderProgram> program,
		bool instancing,
		const glm::vec3& pos = glm::vec3(1.0f),
		const glm::vec3& rot = glm::vec3(1.0f),
		float amountOfRotation = 1.0f,
		const glm::vec3& scale = glm::vec3(1.0f),
		bool bDrawRelativeToCamera = false,
		bool bUseSpotlight = false);

private:
	void createMesh(bool instancing);

	GLuint m_vao, m_vbo, m_ebo;
	Transform m_transform;
	glm::mat4* m_modelMatricesIns;
	float m_totalObjectsIns;
	std::vector<MeshVertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<MeshTexture> m_textures;
};