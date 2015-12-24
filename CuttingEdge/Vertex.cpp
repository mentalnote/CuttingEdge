#include "Vertex.h"

GLuint const Vertex::SIZE = sizeof(float) * 9;

Vertex::Vertex()
{
	this->position = glm::vec3();
	this->normal = glm::vec3();
	this->color = glm::vec3();
}

Vertex::Vertex(glm::vec3 position)
{
	this->position = position;
	this->normal = glm::vec3();
	this->color = glm::vec3();
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal)
{
	this->position = position;
	this->normal = normal;
	this->color = glm::vec3();
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color)
{
	this->position = position;
	this->normal = normal;
	this->color = color;
}
