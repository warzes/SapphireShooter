#pragma once

#include "Font2.h"
#include "Shape2.h"
#include "Engine.h"

class FontRenderer : public Shape2
{
	Font font;
	glm::vec3 color = glm::vec3(0.0f);
	float posX = 0;
	float posY = 0;
	float scale = 1;
	std::string text;

	glm::mat4 getProjectionMatrix() const { return glm::ortho(0.0f, (float)GetEngineDescription().window.width, 0.0f, (float)GetEngineDescription().window.height); }

public:
	FontRenderer(const Font& f);

	virtual void render(std::shared_ptr<ShaderProgram> program);

	void setColor(const glm::vec3& c) { color = c; }
	glm::vec3 getColor() const { return color; }

	void setPosition(const float& x, const float& y) { posX = x; posY = y; }
	glm::vec2 getPosition() const { return glm::vec2(posX, posY); }

	void setScale(const float& s) { scale = s; }
	float getScale() const { return scale; }

	void setText(const std::string& textLine) { text = textLine; }
	std::string getText() const { return text; }
};