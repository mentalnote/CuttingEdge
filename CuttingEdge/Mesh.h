#pragma once
#include "gl_includes.h"
#include <vector>
#include <string>
#include "Component.h"
#include "Drawable.h"
#include <glm/gtc/type_ptr.hpp>
#include "Vertex.h"
#include "Material.h"

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

	Material* material;

	explicit Mesh(MeshData* meshData);

	// Gets mesh data
	MeshData* GetMeshData() const;

	Material* GetMaterial() override;

	// Internal function only for setting reference to component which owns this object
	void _SetComponent(Component* component) override;

	Component* GetComponent() override;

	void Draw() override;

private:
	MeshData* meshData;

	Component* component;
};