#include "Mesh.h"

Mesh::Mesh(Mesh::MeshData* meshData) {
	this->meshData = meshData;
}

Mesh::MeshData* const Mesh::GetMeshData() {
	return this->meshData;
}

void Mesh::Draw() {
	glBindVertexArray(this->meshData->vao);	
	glDrawElements(GL_TRIANGLES, this->meshData->elementsCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}