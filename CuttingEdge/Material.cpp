#include "Material.h"
#include <iostream>

GLuint Material::boundProgram = 0;

Material::Material() : shader()
{
}

Material::Material(ShaderProgram* shader)
{
	this->shader = shader;
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
	this->floatProperties[name] = data;
}

void Material::SetVector4(std::string name, glm::vec4* data)
{
	this->vec4Properties[name] = data;
}

void Material::SetMatrix4(std::string name, glm::mat4* data)
{
	this->matrix4Properties[name] = data;
}

void Material::SetTexture(std::string name, Texture* data)
{
	this->textureProperties[name] = data;
}

void Material::UpdateUniform(std::string name)
{
	auto uniform = this->shader->propertyMap.at(name);
	if(uniform.second == GL_FLOAT_MAT4)
	{
		glUniformMatrix4fv(uniform.first, 1, GL_FALSE, (GLfloat*)this->matrix4Properties[name]);
	} else if (uniform.second == GL_FLOAT_VEC4)
	{
		glm::vec4 data = *this->vec4Properties[name];
		glUniform4f(uniform.first, data[0], data[1], data[2], data[3]);
	} else if(uniform.second == GL_FLOAT)
	{
		glUniform1f(uniform.first, this->floatProperties[name]);
	} else if(uniform.second == GL_SAMPLER_2D)
	{
		
	}
}

void Material::UpdateAllUniforms()
{
	for(auto prop : this->shader->propertyMap)
	{
		if (prop.second.second == GL_FLOAT_MAT4)
		{
			glUniformMatrix4fv(prop.second.first, 1, GL_FALSE, (GLfloat*)this->matrix4Properties.at(prop.first));
		}
		else if (prop.second.second == GL_FLOAT_VEC4)
		{
			glm::vec4 data = *this->vec4Properties[prop.first];
			glUniform4f(prop.second.first, data[0], data[1], data[2], data[3]);
		}
		else if (prop.second.second == GL_FLOAT)
		{
			glUniform1f(prop.second.first, this->floatProperties[prop.first]);
		}
		else if (prop.second.second == GL_SAMPLER_2D)
		{
//			Texture* texture = this->textureProperties.at(prop.first);
//			glActiveTexture(prop.second.first);
//			texture->Bind();
//			glUniform1i()
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

	for(auto prop : this->shader->propertyMap)
	{
		this->UpdateUniform(prop.first);
	}
}