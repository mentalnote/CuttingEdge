#include "ShaderProgram.h"

const char* ShaderProgram::VERT_EXTENSION = "vert";
const char* ShaderProgram::FRAG_EXTENSION = "frag";
const char* ShaderProgram::GEOM_EXTENSION = "geom";

ShaderProgram::ShaderProgram(std::string name) : name(name), shaders(), programId(), propertyMap()
{
}