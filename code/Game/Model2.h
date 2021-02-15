#pragma once

#include "Mesh2.h"
#include "TextureLoader.h"
#include "Transformationable.h"

class Model2 : public Transformationable
{
	std::vector<Mesh*> meshes;
	std::vector<Mesh::MeshTexture> texturesLoaded;
	std::string directory;
	unsigned* amount;

	//void swap(const Model& model);
	void clear();

protected:
	Assimp::Importer importer;
	const aiScene* scene = nullptr;

private:
	void processNode(const aiNode* node);
	Material loadMaterial(const aiMaterial* mat);
	std::vector<Mesh::MeshTexture> loadMaterialTextures(const aiMaterial* mat, const aiTextureType& type, const std::string& typeName);

protected:
	virtual Mesh* processMesh(const aiMesh* aMesh);
	std::vector<Mesh::MeshVertex> getVertices(const aiMesh* aMesh);
	std::vector<unsigned> getIndices(const aiMesh* aMesh);
	std::vector<Mesh::MeshTexture> getTextures(const aiMesh* aMesh);
	Material getMaterial(const aiMesh* aMesh, const unsigned& texCount);

public:
	Model2();
	Model2(const std::string& path);
	//Model2(const Model2& model);
	//Model2& operator=(const Model2& model);
	virtual ~Model2();

	virtual void Render(std::shared_ptr<ShaderProgram> program);
	virtual void load(const std::string& path);

	void changeWholeMaterial(const Material& mat);

	const std::vector<Mesh*>& getMeshes() { return meshes; }
};