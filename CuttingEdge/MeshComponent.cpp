#include "MeshComponent.h"

void MeshComponent::SetMaterial(Material* material)
{
	this->mesh->material = material;
}

Material* MeshComponent::GetMaterial()
{
	return this->mesh->GetMaterial();
}

MeshComponent::MeshComponent(Transform* transform, Mesh* mesh, std::string name) : Component(transform, name)
{
	this->mesh = mesh;

	mesh->_SetComponent(this);
	ResourceManager::BufferMesh(mesh->GetMeshData());
}

Drawable* MeshComponent::GetDrawable() const
{
	return this->mesh;
}

Processable* MeshComponent::GetProcessable() const
{
	return nullptr;
}

MeshComponent::~MeshComponent()
{
	if (mesh) {
		free(mesh);
	}
}