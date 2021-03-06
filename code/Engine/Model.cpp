#include "stdafx.h"
#if SE_OPENGL
#include "Model.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "Log.h"

void Model3::Init(const char* path, Camera3D& camera, const char* vs, const char* fs, bool instancing)
{
	m_shaderProgram = ShaderManager::Get().LoadShader("modelShader", vs, fs);
	m_camera = camera;
	m_instancing = instancing;
	loadModel(path);
}

void Model3::Draw(Camera3D& cam, bool bDrawRelativeToCamera)
{
	for (GLuint i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Draw(cam, m_shaderProgram, false, m_position, m_rotation, m_rotationAngle, m_scale, bDrawRelativeToCamera, m_useSpotlight);
	}
}

void Model3::Draw(Camera3D& cam, const glm::vec3& pos, const glm::vec3& rot, float amountOfRotation, const glm::vec3& scale, bool bDrawRelativeToCamera)
{
	for (GLuint i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Draw(cam, m_shaderProgram, false, pos, rot, amountOfRotation, scale, bDrawRelativeToCamera, m_useSpotlight);
	}
}

void Model3::DrawInstanced(Camera3D& cam)
{
	for (GLuint i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Draw(cam, m_shaderProgram, true);
	}
}

void Model3::SetTransform(glm::vec3 pos, glm::vec3 rot, float rotAmountInDegrees, glm::vec3 scale)
{
	m_position = pos;
	m_rotation = rot;
	m_rotationAngle = rotAmountInDegrees;
	m_scale = scale;
}

void Model3::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		SE_LOG(importer.GetErrorString());
		SE_LOG("ERROR: Unable to load model\n");
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model3::processNode(aiNode* node, const aiScene* scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model3::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<MeshVertex> vertices;
	std::vector<GLuint> indices;
	std::vector<MeshTexture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		MeshVertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures, m_instancing);
}

std::vector<MeshTexture> Model3::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<MeshTexture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		GLboolean skip = false;

		for (GLuint j = 0; j < textures_loaded.size(); j++)
		{
			if (textures_loaded[j].path == str)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			MeshTexture texture;

			std::string filename = std::string(str.C_Str());
			filename = directory + '/' + filename;

			texture.tex = TextureManager::Get().LoadTexture(filename, filename.c_str(), true);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}

	return textures;
}

#endif