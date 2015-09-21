#pragma once
#include <SDL_opengl.h>
#include "Drawable.h"
#include <vector>
#include <string>

class Mesh: public Drawable {
public :
	struct MeshData {
		std::string path;
		GLuint vbo;
		GLuint ebo;
		std::vector<GLuint> elements;
		std::vector<GLfloat> vertices;
	};

	Mesh(Mesh::MeshData* meshData);

	void Draw() override;
};