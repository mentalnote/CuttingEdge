#pragma once
#include <string>
#include <utility>
#include <unordered_map>
#include "gl_includes.h"

class ShaderProgram
{
public:

	const static char* VERT_EXTENSION;
	const static char* FRAG_EXTENSION;
	const static char* GEOM_EXTENSION;

	struct Shader
	{
		std::string path;
		GLchar* source;
		GLuint id;
		GLuint type;
	};

	std::string name;

	std::vector<Shader*> shaders;

	GLuint programId;

	std::unordered_map<std::string, std::pair<GLuint, GLuint>> propertyMap;

	explicit ShaderProgram(std::string name);
};