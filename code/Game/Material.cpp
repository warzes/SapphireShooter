#include "stdafx.h"
#include "Material.h"

void Material::render(Program& program)
{
	program.SetVec3("mat.diffuse", diffuse);
	program.SetVec3("mat.ambient", ambient);
	program.SetVec3("mat.specular", specular);
	program.SetFloat("mat.shininess", shininess);
	program.SetInt("mat.isSupportTex", isSupportTex);
	program.SetFloat("mat.transparency", transparency);
}