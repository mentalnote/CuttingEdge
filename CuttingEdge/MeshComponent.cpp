#include "MeshComponent.h"

MeshComponent::MeshComponent(Transform* transform, Mesh* mesh, std::string name) : Component(transform, name)
{
	this->mesh = mesh;
	this->mesh->SetComponent(this);
	ResourceManager::BufferMesh(this->mesh->GetMeshData());
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
		delete mesh;
	}
}
