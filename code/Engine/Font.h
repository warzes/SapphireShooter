#pragma once

#include "ShaderProgram.h"

struct Character
{
	GLuint textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};

class Text
{
public:
	void SetPosition(const glm::vec2 &pos) { m_position = pos; }
	void SetColor(const glm::vec3 &col) { m_color = col; }
	void SetScale(float scale) { m_scale = scale; }
	void SetText(const std::string &text) { m_text = text; }
	void SetSpacing(float space) { m_spacing = space; }

	void Configure(const std::string &font);
	void Render();

private:
	std::map<char, Character> m_characters;
	GLuint m_vao, m_vbo;
	std::shared_ptr<ShaderProgram> m_shader;
	std::string m_text;
	float m_scale, m_spacing;
	glm::vec3 m_color;
	glm::vec2 m_position;
};