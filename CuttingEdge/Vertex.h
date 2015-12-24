#pragma once

#include "gl_includes.h"
#include <glm\gtc\matrix_transform.hpp>

class Vertex {

public:
	GLuint static const SIZE;

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;

	Vertex();
	Vertex(glm::vec3 position);
	Vertex(glm::vec3 position, glm::vec3 normal);
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color);
};