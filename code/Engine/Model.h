#pragma once

#if SE_OPENGL

#include "Mesh.h"
#include "Transformation.h"
#include "ShaderProgram.h"

class Model3
{
public:
	void Init(const char* path, Camera3D& camera, const char* vs, const char* fs, bool instancing);
	void Draw(Camera3D& cam, bool bDrawRelativeToCamera = false);
	void Draw(Camera3D& cam, 
		const glm::vec3& pos = glm::vec3(1.0f), 
		const glm::vec3& rot = glm::vec3(1.0f), 
		float amountOfRotation = 0.0f, 
		const glm::vec3& scale = glm::vec3(1.0f), 
		bool bDrawRelativeToCamera = false);
	void DrawInstanced(Camera3D& cam);

	void SetTransform(glm::vec3 pos, glm::vec3 rot, float rotAmountInDegrees, glm::vec3 scale);
	void SetSpotlight(bool useSpotlight) { m_useSpotlight = useSpotlight; }
	std::shared_ptr<ShaderProgram> GetShaderProgram() { return m_shaderProgram; }

	std::vector<Mesh> meshes;
	GLuint program;
	bool m_instancing = false;
	std::vector<MeshTexture> textures_loaded;

private:
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	bool m_useSpotlight;

	std::string directory;
	std::shared_ptr<ShaderProgram> m_shaderProgram;

	Camera3D m_camera;
	glm::vec3 m_position, m_rotation, m_scale;
	float m_rotationAngle;
};

#endif