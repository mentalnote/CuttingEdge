#include "Primitives.h"

const std::string Primitives::QUAD_NAME = "quad";
const std::vector<Vertex> Primitives::QUAD_VERTS = 
{
	{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
	{glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	{glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
	{glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)}
};

const std::vector<unsigned int> Primitives::QUAD_ELEMENTS = {3, 1, 0, 2, 1, 3};

Mesh* Primitives::CreateQuad()
{
	auto pairQuadData = ResourceManager::GetMeshData(QUAD_NAME);

	Mesh::MeshData* quadData = nullptr;

	if(pairQuadData.first == nullptr)
	{
		quadData = new Mesh::MeshData();
		quadData->name = QUAD_NAME;

		quadData->elements = QUAD_ELEMENTS;

		quadData->vertices = QUAD_VERTS;

		quadData->elementsCount = 6;
		quadData->verticesCount = 4;
	} else
	{
		quadData = pairQuadData.first[0];
	}

	return new Mesh(quadData);
}