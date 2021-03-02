#pragma once

#include "Renderable.h"

class Material : public Renderable
{
public:
	Material(
		const glm::vec3& ambient = glm::vec3(0.0f),
		const glm::vec3& diffuse = glm::vec3(0.0f),
		const glm::vec3& specular = glm::vec3(0.0f),
		const float& shininess = 2,
		const float& opacity = 1,
		const int& supportTex = 0) 
		: m_ambient(ambient)
		, m_diffuse(diffuse),
		m_specular(specular)
		, m_shininess(shininess),
		m_transparency(opacity)
		, m_isSupportTex(supportTex) 
	{
	}

	void Render(std::shared_ptr<ShaderProgram> program) override;

	glm::vec3 GetAmbient() const { return m_ambient; }
	glm::vec3 GetDiffuse() const { return m_diffuse; }
	glm::vec3 GetSpecular() const { return m_specular; }
	float GetShininess() const { return m_shininess; }
	int GetTransparency() const { return m_transparency; }

	void SetAmbient(const glm::vec3& vec) { m_ambient = vec; }
	void SetDiffuse(const glm::vec3& vec) { m_diffuse = vec; }
	void SetSpecular(const glm::vec3& vec) { m_specular = vec; }
	void SetShininess(float value) { m_shininess = value; }
	void SetTransparency(float value) { m_transparency = value; }

	bool IsSupportingTex() const { return m_isSupportTex == 1; }
	void SetSupportTex() { m_isSupportTex = 1; }
	void UnsetSupportTex() { m_isSupportTex = 0; }

private:
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	glm::vec3 m_ambient;
	float m_shininess;
	int m_isSupportTex;
	float m_transparency;
};