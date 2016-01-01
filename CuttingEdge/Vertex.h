#pragma once

#include "gl_includes.h"
#include <glm\gtc\matrix_transform.hpp>

class Vertex {

public:
	GLuint static const SIZE;
	GLuint static const POS_OFFSET;
	GLuint static const NORMAL_OFFSET;
	GLuint static const COLOR_OFFSET;
	GLuint static const UV_OFFSET;

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 uv;

	Vertex();
	explicit Vertex(glm::vec3 position);
	Vertex(glm::vec3 position, glm::vec3 normal);
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color);
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color, glm::vec2 uv);
};