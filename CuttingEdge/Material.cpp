#include "Material.h"
#include <iostream>

GLuint Material::boundProgram = 0;

Material::Material() : materialProperties(), shader()
{
}

Material::Material(ShaderProgram* shader) : materialProperties()
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

void Material::Unbind()
{
	if (Material::boundProgram == 0)
	{
		return;
	}

	glUseProgram(0);
	Material::boundProgram = 0;
}

void Material::SetUniform(PropertyType type, std::string name, void* data)
{
	this->materialProperties[name] = { name, type, data };
	this->UpdateUniform(this->materialProperties[name]);
}

void Material::SetShader(ShaderProgram* shader)
{
	this->shader = shader;

	for(auto prop : this->materialProperties)
	{
		this->UpdateUniform(prop.second);
	}
}

void Material::UpdateUniform(MaterialProperty matProp)
{
	if(this->shader != nullptr)
	{
		if(this->shader->propertyMap.count(matProp.name))
		{
			if(Material::boundProgram != this->shader->programId)
			{
				this->Bind();
			}

			GLuint propId = this->shader->propertyMap[matProp.name].first;
			GLint* ivals;
			GLuint* uivals;
			GLfloat* fvals;
			GLdouble* dvals;

			switch (matProp.type)
			{
			case PropertyType::INT1: 
				glUniform1i(propId, *(GLint*)matProp.value);
				break;
			case PropertyType::INT2:
				ivals = (GLint*)matProp.value;
				glUniform2i(propId, ivals[0], ivals[1]);
				break;
			case PropertyType::INT3: 
				ivals = (GLint*)matProp.value;
				glUniform3i(propId, ivals[0], ivals[1], ivals[2]);
				break;
			case PropertyType::INT4: 
				ivals = (GLint*)matProp.value;
				glUniform4i(propId, ivals[0], ivals[1], ivals[2], ivals[3]);
				break;
			case PropertyType::UINT1: 
				glUniform1ui(propId, *(GLuint*)matProp.value);
				break;
			case PropertyType::UINT2:
				uivals = (GLuint*)matProp.value;
				glUniform2ui(propId, uivals[0], uivals[1]);
				break;
			case PropertyType::UINT3: 
				uivals = (GLuint*)matProp.value;
				glUniform3ui(propId, uivals[0], uivals[1], uivals[2]);
				break;
			case PropertyType::UINT4: 
				uivals = (GLuint*)matProp.value;
				glUniform4ui(propId, uivals[0], uivals[1], uivals[2], uivals[3]);
				break;
			case PropertyType::FLOAT1: 
				glUniform1f(propId, *(GLfloat*)matProp.value);
				break;
			case PropertyType::FLOAT2: 
				fvals = (GLfloat*)matProp.value;
				glUniform2f(propId, fvals[0], fvals[1]);
				break;
			case PropertyType::FLOAT3: 
				fvals = (GLfloat*)matProp.value;
				glUniform3f(propId, fvals[0], fvals[1], fvals[2]);
				break;
			case PropertyType::FLOAT4: 
				fvals = (GLfloat*)matProp.value;
				glUniform4f(propId, fvals[0], fvals[1], fvals[2], fvals[3]);
				break;
			case PropertyType::DOUBLE1: 
				glUniform1d(propId, *(GLdouble*)matProp.value);
				break;
			case PropertyType::DOUBLE2: 
				dvals = (GLdouble*)matProp.value;
				glUniform2d(propId, dvals[0], dvals[1]);
				break;
			case PropertyType::DOUBLE3: 
				dvals = (GLdouble*)matProp.value;
				glUniform3d(propId, dvals[0], dvals[1], dvals[2]);
				break;
			case PropertyType::DOUBLE4: 
				dvals = (GLdouble*)matProp.value;
				glUniform4d(propId, dvals[0], dvals[1], dvals[2], dvals[3]);
				break;
			case PropertyType::MAT2: 
				glUniformMatrix2fv(propId, 1, GL_FALSE, (GLfloat*)matProp.value);
				break;
			case PropertyType::MAT3: 
				glUniformMatrix3fv(propId, 1, GL_FALSE, (GLfloat*)matProp.value);
				break;
			case PropertyType::MAT4: 
				glUniformMatrix4fv(propId, 1, GL_FALSE, (GLfloat*)matProp.value);
				break;
			case PropertyType::MAT2X3: 
				glUniformMatrix2x3fv(propId, 1, GL_FALSE, (GLfloat*)matProp.value);
				break;
			case PropertyType::MAT3X2: 
				glUniformMatrix3x2fv(propId, 1, GL_FALSE, (GLfloat*)matProp.value);
				break;
			case PropertyType::MAT4X2: 
				glUniformMatrix4x2fv(propId, 1, GL_FALSE, (GLfloat*)matProp.value);
				break;
			case PropertyType::MAT2X4: 
				glUniformMatrix2x4fv(propId, 1, GL_FALSE, (GLfloat*)matProp.value);
				break;
			case PropertyType::MAT3X4: 
				glUniformMatrix3x4fv(propId, 1, GL_FALSE, (GLfloat*)matProp.value);
				break;
			case PropertyType::MAT4X3: 
				glUniformMatrix4x3fv(propId, 1, GL_FALSE, (GLfloat*)matProp.value);
				break;
			case PropertyType::SAMPLER2D: break;
			default: break;
			}
		} else if(matProp.type == PropertyType::SAMPLER2D)
		{
			if (Material::boundProgram != this->shader->programId)
			{
				this->Bind();
			}

			std::string bindString = matProp.name;
			bindString.erase(0, 7);
			GLuint texBinding = std::stoi(bindString);

			glActiveTexture(GL_TEXTURE0 + texBinding);
			glBindTexture(GL_TEXTURE_2D, *(GLuint*)matProp.value);
		}
	}
}