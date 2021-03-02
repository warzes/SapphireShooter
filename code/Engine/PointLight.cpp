#include "stdafx.h"
#include "PointLight.h"
//-----------------------------------------------------------------------------
void PointLight::Configure(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float constant, float linear, float quadratic)
{
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
	m_constant = constant;
	m_linear = linear;
	m_quadratic = quadratic;
}
//-----------------------------------------------------------------------------