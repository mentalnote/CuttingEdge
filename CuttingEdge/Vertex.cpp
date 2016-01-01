#include "Vertex.h"

GLuint const Vertex::SIZE = sizeof(GLfloat) * 11;
GLuint const Vertex::POS_OFFSET = 0;
GLuint const Vertex::NORMAL_OFFSET = sizeof(GLfloat) * 3;
GLuint const Vertex::COLOR_OFFSET = sizeof(GLfloat) * 6;
GLuint const Vertex::UV_OFFSET = sizeof(GLfloat) * 9;

Vertex::Vertex() : position(), normal(), color(), uv()
{
}

Vertex::Vertex(glm::vec3 position) : normal(), color(), uv()
{
	this->position = position;
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal) : color(), uv()
{
	this->position = position;
	this->normal = normal;
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color) : uv()
{
	this->position = position;
	this->normal = normal;
	this->color = color;
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color, glm::vec2 uv)
{
	this->position = position;
	this->normal = normal;
	this->color = color;
	this->uv = uv;
}