#pragma once

class DirectionalLight
{
public:
	void Configure(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular);

	void SetDirection(const glm::vec3 &dir) { m_direction = dir; }
	void SetColour(const glm::vec3 &col) { m_colour = col; }

	glm::vec3& GetDirection() { return m_direction; }
	glm::vec3& GetAmbient() { return m_ambient; }
	glm::vec3& GetDiffuse() { return m_diffuse; }
	glm::vec3& GetSpecular() { return m_specular; }
	glm::vec3& GetColour() { return m_colour; }
private:
	glm::vec3 m_direction;
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	glm::vec3 m_colour;
};