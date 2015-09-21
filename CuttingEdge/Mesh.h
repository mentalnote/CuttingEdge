#pragma once
#include "gl_includes.h"
#include "Drawable.h"
#include <vector>
#include <string>

class Mesh: public Drawable {
public :
	struct MeshData {
		std::string path;
		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		std::vector<GLuint> elements;
		std::vector<GLfloat> vertices;
		GLuint elementsCount;
		GLuint verticesCount;
	};

	MeshData* const GetMeshData();

	Mesh(Mesh::MeshData* meshData);

	void Draw() override;

private:
	MeshData* meshData;
};