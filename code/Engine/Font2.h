#pragma once

#if SE_OPENGL

#include "Texture.h"

class Font
{
public:
	struct Character
	{
		Texture texture;
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned advance;
	};

private:
	std::string font;
	std::map<char, Character> characters;

public:
	Font() { ; }
	Font(const std::string& fontPath);

	void load(const std::string& fontPath);

	std::string getFontPath() const { return font; }
	std::map<char, Character>& getCharacters() { return characters; }
};

#endif