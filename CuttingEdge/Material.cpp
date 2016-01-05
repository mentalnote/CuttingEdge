#include "Material.h"
#include <iostream>

GLuint Material::boundProgram = 0;

Material::Material() : shader()
{
}

Material::Material(ShaderProgram* shader) : propTest(shader->properties.size()), propertyData(shader->properties.size())
{
	this->shader = shader;

	for (int i = 0; i < shader->properties.size(); i++)
	{
		this->propertyMap[shader->properties[i].first] = i;

		this->propTest[i] = shader->properties[i].first;
	}
}

void Material::Bind()
{
	if(Material::boundProgram == this->shader->programId)
	{
		return;
	}

	glUseProgram(this->shader->programId);
	Material::boundProgram = this->shader->programId;
}

void Material::SetFloat(std::string name, GLfloat data)
{
	this->propertyData[this->propertyMap[name]] = &data;
}

void Material::SetVector4(std::string name, glm::vec4* data)
{
	this->propertyData[this->propertyMap[name]] = &data;
}

void Material::SetMatrix4(std::string name, glm::mat4* data)
{
	for (int i = 0; i < propTest.size(); i++)
	{
		if(propTest[i] == name)
		{
			this->propertyData[i] = &data[0][0];
			return;
		}
	}
	
	//this->propertyData[this->propertyMap[name]] = &data[0][0];
}

void Material::SetTexture(std::string name, Texture* data)
{
	this->propertyData[this->propertyMap[name]] = &data;
}

void Material::UpdateAllUniforms()
{
	for (int i = 0; i < this->shader->properties.size(); i++)
	{
		GLuint propType = this->shader->properties[i].second;

		switch (propType) {
		case GL_FLOAT_MAT4:
			glUniformMatrix4fv(i, 1, GL_FALSE, (GLfloat*)this->propertyData[i]);
			break;
		case GL_FLOAT_VEC4:
		{
			glm::vec4 data = *(glm::vec4*)this->propertyData[i];
			glUniform4f(i, data[0], data[1], data[2], data[3]);
			break;
		}
		case GL_FLOAT:
			glUniform1f(i, *(GLint*)this->propertyData[i]);
			break;
		case GL_SAMPLER_2D:
			break;
		}
	}
}

void Material::Unbind()
{
	if (Material::boundProgram == this->shader->programId)
	{
		glUseProgram(0);
		Material::boundProgram = 0;
	}
}

void Material::SetShader(ShaderProgram* shader)
{
	this->shader = shader;

	this->propertyData.clear();
	this->propertyMap.clear();

	this->propertyData.resize(shader->properties.size());

	for (int i = 0; i < shader->properties.size(); i++)
	{
		this->propertyMap[shader->properties[i].first] = i;
	}

	this->UpdateAllUniforms();
}