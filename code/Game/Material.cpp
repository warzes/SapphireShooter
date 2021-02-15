#include "stdafx.h"
#include "Material.h"
//-----------------------------------------------------------------------------
void Material::Render(std::shared_ptr<ShaderProgram> program)
{
	program->SetVector3f("mat.diffuse", m_diffuse);
	program->SetVector3f("mat.ambient", m_ambient);
	program->SetVector3f("mat.specular", m_specular);
	program->SetFloat("mat.shininess", m_shininess);
	program->SetInteger("mat.isSupportTex", m_isSupportTex);
	program->SetFloat("mat.transparency", m_transparency);
}
//-----------------------------------------------------------------------------