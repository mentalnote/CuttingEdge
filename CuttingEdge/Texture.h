#pragma once
#include "gl_includes.h"

class Texture
{
public:
	struct TextureData {
		std::string path;
		std::string name;
		GLint width;
		GLint height;
		GLint components;
		GLuint index;
		unsigned char* data;
	};

	TextureData* data;

	explicit Texture(TextureData* data);
};