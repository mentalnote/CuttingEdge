#include "Mesh.h"

std::string const POS_ATTRIB_NAME = "position";
std::string const NORMAL_ATTRIB_NAME = "normal";
std::string const COLOR_ATTRIB_NAME = "color";

Mesh::Mesh(MeshData* meshData) {
	this->meshData = meshData;
	this->component = nullptr;
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