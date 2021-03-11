#pragma once

#if SE_OPENGL

#include "Shape.h"
#include "Texture2D.h"
#include "Transformation.h"
#include "Camera3D.h"
#include "SpotLight.h"
#include "ShaderProgram.h"

// TODO: rename polygon

class Billboard
{
public:
	static Billboard* Create(ShapeType meshType, const std::string& texId, const std::vector<const char*> &shaders, const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scale, const std::string& normalMapping = "blank", bool instancing = false);

	void CreateGameObj(std::vector<Vertex>, GLuint numOfVertices, unsigned int* indices, GLuint numOfIndices, bool bInstancing = false);
	void CreateShaderComponent(const char* vs, const char* fs);

	void SetTransform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);

	void Draw(Camera3D& cam, glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f), bool bNormalMapping = false, SpotLight* spotlight = nullptr, bool postProcessing = false, GLuint postProcessingTexId = 0);
	void Draw(glm::mat4 model, Camera3D& cam, glm::vec3 lightPos);

private:
	GLuint m_numOfVertices, m_numOfIndices;
	GLuint m_vao;
	enum { POSITION_VB, COLOR_VB, UV_VB, NORMALS_VB, TANGENT_VB, ELEMENT_VB, TOTAL_BUFFERS };
	GLuint m_vbo[TOTAL_BUFFERS];

	std::shared_ptr<ShaderProgram> m_shaderComponent;
	std::shared_ptr<Texture2D> m_texture;

	Transform m_transform;
};

#endif