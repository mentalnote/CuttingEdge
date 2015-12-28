#include "Mesh.h"

std::string const POS_ATTRIB_NAME = "position";
std::string const NORMAL_ATTRIB_NAME = "normal";
std::string const COLOR_ATTRIB_NAME = "color";

Mesh::Mesh(MeshData* meshData) {
	this->meshData = meshData;
	this->material = meshData->material;
	this->component = nullptr;
}

Mesh::MeshData* Mesh::GetMeshData() const {
	return this->meshData;
}

Material* Mesh::GetMaterial()
{
	return this->material;
}

void Mesh::_SetComponent(Component * component)
{
	this->component = component;
}

Component* Mesh::GetComponent()
{
	return this->component;
}

void Mesh::Draw() {
	glBindVertexArray(this->meshData->vao);

	glDrawElements(GL_TRIANGLES, this->meshData->elementsCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}