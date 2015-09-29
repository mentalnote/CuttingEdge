#include "Mesh.h"

Mesh::Mesh(MeshData* meshData) {
	this->meshData = meshData;
}

Mesh::MeshData* Mesh::GetMeshData() const {
	return this->meshData;
}

void Mesh::_SetComponent(Component * component)
{
	this->component = component;
}

void Mesh::Draw(Camera* camera) {
	glBindVertexArray(this->meshData->vao);	

	glm::mat4 MVP = camera->GetProjectionMatrix() * camera->GetViewMatrix() * this->component->GetTransform()->GetWorldMatrix();

	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

	glDrawElements(GL_TRIANGLES, this->meshData->elementsCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}