#include "stdafx.h"
#if SE_OPENGL
#include "Mesh.h"
#include "OGLFunc.h"
#include "Game/Player.h" // TODO:
//-----------------------------------------------------------------------------
Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<GLuint>& indices, const std::vector<MeshTexture>& textures, bool instancing)
{
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;
	createMesh(instancing);
}
void Mesh::Draw(Camera3D& camera, std::shared_ptr<ShaderProgram> shaderProgram, bool instancing, const glm::vec3& pos, const glm::vec3& rot, float amountOfRotation, const glm::vec3& scale, bool bDrawRelativeToCamera, bool bUseSpotlight)
{
	shaderProgram->Bind();

	glm::mat4 model(1.0f);
	glm::mat4 translation = glm::translate(pos);
	glm::mat4 rotation = glm::rotate(amountOfRotation, rot);
	glm::mat4 scaleMat = glm::scale(scale);

	if (bDrawRelativeToCamera)
	{
		glm::mat4 invViewMat = glm::inverse(camera.GetViewMatrix());
		model = invViewMat * translation * rotation * scaleMat;
	}
	else
	{
		model = translation * rotation * scaleMat;
	}

	shaderProgram->SetMatrix4("model", model);
	shaderProgram->SetVector3f("lightPos", glm::vec3(camera.GetCameraPos().x, camera.GetCameraPos().y + 5.0f, camera.GetCameraPos().z));
	shaderProgram->SetVector3f("viewPos", glm::vec3(camera.GetCameraPos().x, camera.GetCameraPos().y, camera.GetCameraPos().z));
	shaderProgram->SetBool("EnableSpotlight", bUseSpotlight);

	if (Player::Get().GetSpotLight() != nullptr && bUseSpotlight)
	{
		shaderProgram->SetVector3f("spotlight.position", Player::Get().GetSpotLight()->GetPosition());
		shaderProgram->SetVector3f("spotlight.direction", Player::Get().GetSpotLight()->GetDirection());
		shaderProgram->SetVector3f("spotlight.diffuse", Player::Get().GetSpotLight()->GetDiffuse());
		shaderProgram->SetVector3f("spotlight.specular", Player::Get().GetSpotLight()->GetSpecular());
		shaderProgram->SetFloat("spotlight.constant", Player::Get().GetSpotLight()->GetConstant());
		shaderProgram->SetFloat("spotlight.linear", Player::Get().GetSpotLight()->GetLinear());
		shaderProgram->SetFloat("spotlight.quadratic", Player::Get().GetSpotLight()->GetQuadratic());
		shaderProgram->SetFloat("spotlight.cutOff", glm::cos(glm::radians(Player::Get().GetSpotLight()->GetCutOff())));
		shaderProgram->SetFloat("spotlight.outerCutOff", glm::cos(glm::radians(Player::Get().GetSpotLight()->GetOuterCutOff())));
	}

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	for (unsigned int i = 0; i < m_textures.size(); ++i)
	{
		std::string number;
		std::string name = m_textures[i].type;

		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		glm::mat4 projMat = camera.GetProjectionMatrix();
		glm::mat4 viewMat = camera.GetViewMatrix();

		shaderProgram->SetMatrix4("projection", projMat);
		shaderProgram->SetMatrix4("view", viewMat);
		shaderProgram->SetInteger((name + number).c_str(), i);

		m_textures[i].tex->Bind(i);
	}

	glBindVertexArray(m_vao);

	if (instancing)
	{
		glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0, m_totalObjectsIns);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);

	// Unbind textures 
	for (GLuint i = 0; i < m_textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
//-----------------------------------------------------------------------------
void Mesh::createMesh(bool instancing)
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MeshVertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)offsetof(MeshVertex, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (GLvoid*)offsetof(MeshVertex, texCoords));

	if (instancing)
	{
		m_totalObjectsIns = 20000;
		unsigned int amount = m_totalObjectsIns;
		m_modelMatricesIns = new glm::mat4[amount];
		float radius = 400.0f;
		float offset = 60.0f;

		for (unsigned int i = 0; i < amount; ++i)
		{
			glm::mat4 model(1.0f);
			float x = 0.0f, y = 0.0f, z = 0.0f;

			float angle = (float)i / (float)amount * 360.0f;
			float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset + 300.0f;
			x = sin(angle) * radius + displacement;

			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset + 40.0f;
			y = displacement * 3.7f;

			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset + 390.0f;
			z = cos(angle) * radius + displacement;

			model = glm::translate(model, glm::vec3(x, y, z));

			float scale = (rand() % 25) / 100.0f + 0.35f;
			model = glm::scale(model, glm::vec3(scale));

			float rotAngle = (rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.5f, 0.7f, 0.9f));

			m_modelMatricesIns[i] = model;
		}

		unsigned int instanceVBO;
		glGenBuffers(1, &instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * amount, &m_modelMatricesIns[0], GL_STATIC_DRAW);

		for (unsigned int i = 0; i < 1; ++i)
		{
			glBindVertexArray(m_vao);

			GLsizei mat4Size = sizeof(glm::mat4);
			glEnableVertexAttribArray(3);
			glEnableVertexAttribArray(4);
			glEnableVertexAttribArray(5);
			glEnableVertexAttribArray(6);

			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, mat4Size, (GLvoid*)0);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, mat4Size, (GLvoid*)(sizeof(glm::vec4)));
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, mat4Size, (GLvoid*)(2 * sizeof(glm::vec4)));
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, mat4Size, (GLvoid*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}
	}

	glBindVertexArray(0);
}

#endif