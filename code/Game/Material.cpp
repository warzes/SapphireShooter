#include "stdafx.h"
#include "Material.h"

void Material::Render(std::shared_ptr<ShaderProgram> program)
{
	program->SetVector3f("mat.diffuse", diffuse);
	program->SetVector3f("mat.ambient", ambient);
	program->SetVector3f("mat.specular", specular);
	program->SetFloat("mat.shininess", shininess);
	program->SetInteger("mat.isSupportTex", isSupportTex);
	program->SetFloat("mat.transparency", transparency);
}