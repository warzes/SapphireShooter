#pragma once

#include "Renderable.h"

// TODO: надо не наследоваться а включать

class Transformationable : public Renderable
{
public:
	void Render(std::shared_ptr<ShaderProgram> program) override
	{
		program->SetMatrix4("model", GetTransformMatrix());
	}

	glm::mat4 GetTransformMatrix() const
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, m_position);
		matrix = glm::scale(matrix, m_scale);
		matrix = matrix * m_rotation;
		return matrix;
	}

	void Rotate(const float& angle, const glm::vec3& vec) { m_rotation = glm::rotate(m_rotation, glm::radians(angle), vec); }
	void RotateX(const float& angle) { Rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f)); }
	void RotateY(const float& angle) { Rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f)); }
	void RotateZ(const float& angle) { Rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f)); }
	
	void Translate(const glm::vec3& translate) { m_position += translate; }
	void SetScale(const glm::vec3& vec) { m_scale = vec; }

	void SetPosition(const glm::vec3& vec) { m_position = vec; }

	glm::vec3 GetPosition() const { return m_position; }
	glm::vec3 GetScale() const { return m_scale; }
	glm::mat4 GetRotation() const { return m_rotation; }

private:
	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_scale = glm::vec3(1.0f);
	glm::mat4 m_rotation = glm::mat4(1.0f);
};