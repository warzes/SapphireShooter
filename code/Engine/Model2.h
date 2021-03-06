#pragma once

#if SE_OPENGL

#include "Mesh2.h"
#include "TextureLoader.h"
#include "Transformationable.h"

class Model2 : public Transformationable
{
	std::vector<Mesh2*> meshes;
	std::vector<Mesh2::MeshTexture> texturesLoaded;
	std::string directory;
	unsigned* amount;

	void clear();

protected:
	Assimp::Importer importer;
	const aiScene* scene = nullptr;

private:
	void processNode(const aiNode* node);
	Material2 loadMaterial(const aiMaterial* mat);
	std::vector<Mesh2::MeshTexture> loadMaterialTextures(const aiMaterial* mat, const aiTextureType& type, const std::string& typeName);

protected:
	virtual Mesh2* processMesh(const aiMesh* aMesh);
	std::vector<Mesh2::MeshVertex> getVertices(const aiMesh* aMesh);
	std::vector<unsigned> getIndices(const aiMesh* aMesh);
	std::vector<Mesh2::MeshTexture> getTextures(const aiMesh* aMesh);
	Material2 getMaterial(const aiMesh* aMesh, const unsigned& texCount);

public:
	Model2();
	Model2(const std::string& path);
	virtual ~Model2();

	virtual void Render(std::shared_ptr<ShaderProgram> program);
	virtual void load(const std::string& path);

	void changeWholeMaterial(const Material2& mat);

	const std::vector<Mesh2*>& getMeshes() { return meshes; }
};

#endif