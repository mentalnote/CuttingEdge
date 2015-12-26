#pragma once
#include "gl_includes.h"
#include <vector>
#include <string>
#include "Component.h"
#include "Camera.h"
#include "Drawable.h"
#include <glm/gtc/type_ptr.hpp>
#include "Vertex.h"

class Material;

class Mesh: public Drawable {
public :
	struct MeshData {
		std::string path;
		std::string name;
		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		std::vector<GLuint> elements;
		std::vector<Vertex> vertices;
		GLuint elementsCount;
		GLuint verticesCount;
		Material* material;
	};

	static std::string const POS_ATTRIB_NAME;
	static std::string const NORMAL_ATTRIB_NAME;
	static std::string const COLOR_ATTRIB_NAME;

	explicit Mesh(MeshData* meshData);

	// Gets mesh data
	MeshData* GetMeshData() const;

	// Internal function only for setting reference to component which owns this object
	void _SetComponent(Component* component);

	void Draw(Camera* camera) override;

private:
	MeshData* meshData;

	Component* component;
};