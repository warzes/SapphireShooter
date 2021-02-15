#pragma once

// TODO: old

class Transform
{
public:
	Transform(
		const glm::vec3& pos = glm::vec3(), 
		const glm::vec3& rot = glm::vec3(), 
		const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f)) 
		: m_position(pos)
		, m_rotation(rot)
		, m_scale(scale)
	{
	}

	glm::mat4 GetModel() const
	{
		glm::mat4 posMatrix = glm::translate(m_position);
		glm::mat4 rotXMatrix = glm::rotate(glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotYMatrix = glm::rotate(glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotZMatrix = glm::rotate(glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 scaleMatrix = glm::scale(m_scale);
		glm::mat4 rotMatrix = rotXMatrix * rotYMatrix * rotZMatrix;

		return (posMatrix * rotMatrix * scaleMatrix);
	}

	const glm::vec3& GetPosition() const { return m_position; };
	const glm::vec3& GetRotation() const { return m_rotation; };
	const glm::vec3& GetScale() const { return m_scale; };

	void SetPosition(const glm::vec3& pos) { m_position = pos; };
	void SetRotation(const glm::vec3& rot) { m_rotation = rot; };
	void SetScale(const glm::vec3& scale) { m_scale = scale; };

	void Translate(const glm::vec3& translate) { m_position += translate; }

	void RotateX(float angle) { m_rotation.x = angle; }
	void RotateY(float angle) { m_rotation.y = angle; }
	void RotateZ(float angle) { m_rotation.z = angle; }

private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
};