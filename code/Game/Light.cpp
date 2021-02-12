#include "stdafx.h"
#include "Light.h"

unsigned Light::amount = 0;

void Light::generateShadowTransforms()
{
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, shadowFar);
	shadowTransforms.clear();
	shadowTransforms.push_back(projection * glm::lookAt(position, position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(projection * glm::lookAt(position, position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(projection * glm::lookAt(position, position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(projection * glm::lookAt(position, position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(projection * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(projection * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
}

void Light::render(std::shared_ptr<ShaderProgram> program)
{
	using std::string;
	string prefix = "lights[" + std::to_string(getIndex()) + "]";
	program->SetVector3f(string(prefix + ".position").c_str(), position);
	program->SetVector3f(string(prefix + ".ambient").c_str(), ambient);
	program->SetVector3f(string(prefix + ".diffuse").c_str(), diffuse);
	program->SetVector3f(string(prefix + ".specular").c_str(), specular);
	program->SetFloat(string(prefix + ".constant").c_str(), constant);
	program->SetFloat(string(prefix + ".linear").c_str(), linear);
	program->SetFloat(string(prefix + ".quadratic").c_str(), quadratic);
}