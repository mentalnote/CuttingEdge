#include "Material.h"
#include <iostream>

GLuint Material::boundProgram = 0;

Material::Material() : shader()
{
}

Material::Material(ShaderProgram* shader) : propTest(shader->properties.size()), propertyData(shader->properties.size())
{
	this->shader = shader;

	for (unsigned int i = 0; i < shader->properties.size(); i++)
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

void Material::SetMatrix4(std::string* name, glm::mat4* data)
{
	for (unsigned int i = 0; i < propTest.size(); i++)
	{
		if(propTest[i] == *name)
		{
			this->propertyData[i] = &data[0][0];
			return;
		}
	}
	
	//this->propertyData[this->propertyMap[name]] = &data[0][0];
}

void Material::SetTexture(std::string name, Texture* data)
{
	this->propertyData[this->propertyMap[name]] = &data->data->index;
}

void Material::UpdateAllUniforms()
{
	unsigned int samplerCount = 0;

	for (unsigned int i = 0; i < this->shader->properties.size(); i++)
	{
		auto propType = &this->shader->properties[i];
		void* pData = this->propertyData[i];

		if(pData == nullptr)
		{
			continue;
		}

		switch (propType->second) {
		case GL_FLOAT_MAT4:
			glUniformMatrix4fv(i, 1, GL_FALSE, (GLfloat*)pData);
			break;
		case GL_FLOAT_VEC4:
		{
			glm::vec4 data = *(glm::vec4*)pData;
			glUniform4f(i, data[0], data[1], data[2], data[3]);
			break;
		}
		case GL_FLOAT:
			glUniform1f(i, *(GLfloat*)pData);
			break;
		case GL_SAMPLER_2D:
			glActiveTexture(samplerCount);
			glBindTexture(GL_TEXTURE_2D, *(GLuint*)pData);
			glUniform1i(i, samplerCount);
			++samplerCount;
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

	for (unsigned int i = 0; i < shader->properties.size(); i++)
	{
		this->propertyMap[shader->properties[i].first] = i;
	}

	this->UpdateAllUniforms();
}