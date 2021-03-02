#pragma once

class PointLight
{
public:
	void Configure(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float constant, float linear, float quadratic);

	void SetPosition(const glm::vec3 &pos) { m_position = pos; }
	void SetLightColour(const glm::vec3 &col) { m_lightColour = col; }

	glm::vec3& GetPos() { return m_position; }
	glm::vec3& GetColour() { return m_lightColour; }
	glm::vec3& GetAmbient() { return m_ambient; }
	glm::vec3& GetDiffuse() { return m_diffuse; }
	glm::vec3& GetSpecular() { return m_specular; }
	float& GetConstant() { return m_constant; }
	float& GetLinear() { return m_linear; }
	float& GetQuadratic() { return m_quadratic; }

private:
	glm::vec3 m_position;
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	glm::vec3 m_lightColour;

	float m_constant;
	float m_linear;
	float m_quadratic;
};