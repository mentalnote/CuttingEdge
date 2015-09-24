#pragma once
#include "gl_includes.h"
#include <vector>
#include <string>
#include "Component.h"
#include "Camera.h"
#include "Drawable.h"
#include <glm/gtc/type_ptr.hpp>

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

	explicit Mesh(MeshData* meshData);

	// Gets mesh data
	MeshData* GetMeshData() const;

	// Only for setting reference to component which owns this object, 
	// will not set if component parameter GetDrawable method doesn't return this
	void SetComponent(Component* component);

	void Draw(Camera* camera) override;

private:
	MeshData* meshData;

	Component* component;
};