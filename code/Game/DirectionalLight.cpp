#include "stdafx.h"
#include "DirectionalLight.h"
//-----------------------------------------------------------------------------
void DirectionalLight::Configure(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular)
{
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
}
//-----------------------------------------------------------------------------