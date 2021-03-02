#pragma once

class SpotLight
{
public:
	void Configure(const glm::vec3 &diffuse, const glm::vec3 &specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff);
	void SetPosition(const glm::vec3 &pos) { m_position = pos; }
	void SetDirection(const glm::vec3 &dir) { m_direction = dir; }

	glm::vec3& GetPosition() { return m_position; }
	glm::vec3& GetDirection() { return m_direction; }
	glm::vec3& GetDiffuse() { return m_diffuse; }
	glm::vec3& GetSpecular() { return m_specular; }
	float& GetCutOff() { return m_cutOff; }
	float& GetOuterCutOff() { return m_outerCutOff; }
	float& GetConstant() { return m_constant; }
	float& GetLinear() { return m_linear; }
	float& GetQuadratic() { return m_quadratic; }

private:
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

	float m_cutOff;
	float m_outerCutOff;
	float m_constant;
	float m_linear;
	float m_quadratic;
};