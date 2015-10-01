#include "ShaderProgram.h"

const char* ShaderProgram::VERT_EXTENSION = "vert";
const char* ShaderProgram::FRAG_EXTENSION = "frag";
const char* ShaderProgram::GEOM_EXTENSION = "geom";

ShaderProgram::ShaderProgram(std::string name)
{
	this->name = name;
	this->programId = 0;

	this->shaders = std::vector<Shader*>();

	this->propertyMap = std::unordered_map<std::string, std::tuple<GLint, GLuint>>();
}