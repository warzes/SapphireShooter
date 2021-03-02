#include "stdafx.h"
#include "Billboard.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "OGLFunc.h"

Billboard* Billboard::Create(ShapeType meshType, const std::string& textureId, const std::vector<const char*> &shaders, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale, const std::string& normalMapping, bool bIsInstancing)
{
	Shape* shape = Shape::CreateShape(meshType);
	shape->InitVertexData();

	Billboard* polygon = new Billboard();
	polygon->CreateGameObj(shape->GetVertexData(), shape->GetVertexDataCount(), shape->GetIndexData(), shape->GetIndexDataCount(), bIsInstancing);
	polygon->CreateShaderComponent(shaders.at(0), shaders.at(1));

	polygon->m_texture = TextureManager::Get().LoadTexture(textureId.c_str(), textureId.c_str(), true);

	polygon->SetTransform(pos, rot, scale);

	delete shape;
	shape = nullptr;

	return polygon;
}

void Billboard::CreateGameObj(std::vector<Vertex> verts, GLuint numOfVertices, unsigned int* indices, GLuint numOfIndices, bool bInstancing)
{
	m_numOfVertices = numOfVertices;
	m_numOfIndices = numOfIndices;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	std::vector<glm::vec3> vertexPos;
	std::vector<glm::vec3> vertexCol;
	std::vector<glm::vec2> vertexUv;
	std::vector<glm::vec3> vertexNorms;
	std::vector<glm::vec3> vertexTan;

	vertexPos.reserve(m_numOfVertices);
	vertexUv.reserve(m_numOfVertices);
	vertexCol.reserve(m_numOfVertices);
	vertexNorms.reserve(m_numOfVertices);
	vertexTan.reserve(m_numOfVertices);

	for (size_t i = 0; i < numOfVertices; ++i)
	{
		vertexPos.push_back(verts[i].GetPos());
		vertexCol.push_back(verts[i].GetCol());
		vertexUv.push_back(verts[i].GetTex());
		vertexNorms.push_back(verts[i].GetNormals());
		vertexTan.push_back(verts[i].GetTangent());
	}

	glGenBuffers(TOTAL_BUFFERS, m_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(vertexPos[0]), &vertexPos[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[COLOR_VB]);
	glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(vertexCol[0]), &vertexCol[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[UV_VB]);
	glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(vertexUv[0]), &vertexUv[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[NORMALS_VB]);
	glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(vertexNorms[0]), &vertexNorms[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[TANGENT_VB]);
	glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(vertexTan[0]), &vertexTan[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[ELEMENT_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfIndices * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

	if (bInstancing)
	{
		// Note to self: Optimize rendering by doing some instancing stuff for the basic shapes here in the future
	}

	glBindVertexArray(0);
}

void Billboard::CreateShaderComponent(const char* vs, const char* fs)
{
	m_shaderComponent = ShaderManager::Get().LoadShader(std::string(vs) + fs, vs, fs);
}

void Billboard::SetTransform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	m_transform.SetPosition(pos);
	m_transform.SetScale(scale);
	m_transform.SetRotation(rot);
}

void Billboard::Draw(glm::mat4 model, Camera3D& cam, glm::vec3 lightPos)
{
	m_shaderComponent->Bind();
	m_texture->Bind();
	m_shaderComponent->SetMatrix4("projection", cam.GetProjectionMatrix());
	m_shaderComponent->SetMatrix4("view", cam.GetViewMatrix());
	m_shaderComponent->SetMatrix4("model", model);
	m_shaderComponent->SetVector3f("lightPos", lightPos);
	m_shaderComponent->SetVector3f("viewPos", cam.GetCameraPos());

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_numOfIndices, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	m_shaderComponent->UnBind();
}

void Billboard::Draw(Camera3D& camera, glm::vec3 lightPos, bool normalMapping, SpotLight* spotlight, bool postProcessing, GLuint postProcessingTexId)
{
	m_shaderComponent->Bind();

	if (normalMapping)
	{
		//m_shaderComponent->SetInteger("meshTexture", 0);
		//m_shaderComponent->SetInteger("normalMap", 1);
		//m_texture->Bind();
		//m_texture->Bind(1); // TODO:
	}
	else if (postProcessing)
	{
		glBindTexture(GL_TEXTURE_2D, postProcessingTexId);
	}
	else
	{
		m_texture->Bind();
	}

	m_shaderComponent->SetMatrix4("projection", camera.GetProjectionMatrix());
	m_shaderComponent->SetMatrix4("view", camera.GetViewMatrix());
	m_shaderComponent->SetMatrix4("model", m_transform.GetModel());
	m_shaderComponent->SetVector3f("lightPos", lightPos);
	m_shaderComponent->SetVector3f("viewPos", camera.GetCameraPos());

	if (spotlight != nullptr)
	{
		m_shaderComponent->SetVector3f("spotlight.position", spotlight->GetPosition());
		m_shaderComponent->SetVector3f("spotlight.direction", spotlight->GetDirection());
		m_shaderComponent->SetVector3f("spotlight.diffuse", spotlight->GetDiffuse());
		m_shaderComponent->SetVector3f("spotlight.specular", spotlight->GetSpecular());
		m_shaderComponent->SetFloat("spotlight.constant", spotlight->GetConstant());
		m_shaderComponent->SetFloat("spotlight.linear", spotlight->GetLinear());
		m_shaderComponent->SetFloat("spotlight.quadratic", spotlight->GetQuadratic());
		m_shaderComponent->SetFloat("spotlight.cutOff", glm::cos(glm::radians(spotlight->GetCutOff())));
		m_shaderComponent->SetFloat("spotlight.outerCutOff", glm::cos(glm::radians(spotlight->GetOuterCutOff())));
	}

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_numOfIndices, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	m_shaderComponent->UnBind();
}