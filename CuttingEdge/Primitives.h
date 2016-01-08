#pragma once
#include <vector>
#include "ResourceManager.h"
#include "Mesh.h"
#include "Vertex.h"

class Primitives
{
	static const std::string QUAD_NAME;
	static const std::vector<Vertex> QUAD_VERTS;
	static const std::vector<unsigned int> QUAD_ELEMENTS;

public:
	static Mesh* CreateQuad();
};
