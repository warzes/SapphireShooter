#pragma once

#include "Material.h"
#include "Geometry.h"
#include "Texture.h"

class Mesh2 : public Geometry2
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
	Material2 material;

	void swap(const Mesh2& mesh);
	void clear();
	void initMesh();

public:
	Mesh2(const std::vector<MeshVertex>& verts, const std::vector<unsigned>& inds, const std::vector<MeshTexture>& texs, const Material2& mat);
	Mesh2(const Mesh2& mesh);
	Mesh2& operator=(const Mesh2& mesh);
	virtual ~Mesh2();

	virtual void Render(std::shared_ptr<ShaderProgram> program);

	void setMaterial(const Material2& mat) { material = mat; }
	const Material2& getMaterial() const { return material; }
	const std::vector<MeshVertex>& getVertices() const { return vertices; }
	const std::vector<unsigned>& getIndices() const { return indices; }
	const std::vector<MeshTexture>& getTextures() const { return textures; }
};