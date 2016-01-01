#pragma once

#include <unordered_map>
#include "Texture.h"
#include <glm\gtc\matrix_transform.hpp>
#include "ShaderProgram.h"

class Shader;

class Material
{
public:

	static GLuint boundProgram;

	std::unordered_map<std::string, GLfloat> floatProperties;
	std::unordered_map<std::string, glm::vec4*> vec4Properties;
	std::unordered_map<std::string, glm::mat4*> matrix4Properties;
	std::unordered_map<std::string, Texture*> textureProperties;

	ShaderProgram* shader;
	
	Material();
	explicit Material(ShaderProgram* shader);

	void Bind();
	void Unbind();

	void SetFloat(std::string name, GLfloat data);
	void SetVector4(std::string name, glm::vec4* data);
	void SetMatrix4(std::string name, glm::mat4* data);
	void SetTexture(std::string name, Texture* data);

	void SetShader(ShaderProgram* shader);

	void UpdateUniform(std::string name);
	void UpdateAllUniforms();
};
