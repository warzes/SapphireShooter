#pragma once

#include "Material.h"
#include "Shape2.h"
#include "Texture.h"

class Mesh : public Shape2
{
public:
	struct MeshTexture
	{
		Texture texture;
		std::string type;
		std::string path;
	};

	struct MeshVertex
	{
		glm::vec3 position;
		glm::vec2 texCoords;
		glm::vec3 normal;
	};

private:
	unsigned EBO;
	std::vector<MeshVertex> vertices;
	std::vector<unsigned> indices;
	std::vector<MeshTexture> textures;
	Material material;

	void swap(const Mesh& mesh);
	void clear();
	void initMesh();

public:
	Mesh(const std::vector<MeshVertex>& verts, const std::vector<unsigned>& inds, const std::vector<MeshTexture>& texs, const Material& mat);
	Mesh(const Mesh& mesh);
	Mesh& operator=(const Mesh& mesh);
	virtual ~Mesh();

	virtual void render(Program& program);

	void setMaterial(const Material& mat) { material = mat; }
	const Material& getMaterial() const { return material; }
	const std::vector<MeshVertex>& getVertices() const { return vertices; }
	const std::vector<unsigned>& getIndices() const { return indices; }
	const std::vector<MeshTexture>& getTextures() const { return textures; }
};