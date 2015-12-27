#pragma once
#include "Component.h"
#include "Mesh.h"
#include "ResourceManager.h"

class MeshComponent : public Component {
	Mesh* mesh;

public:
	MeshComponent(Transform* transform, Mesh* mesh, std::string name = "MeshComponent");

	void SetMaterial(Material* material);
	Material* GetMaterial();

	Drawable* GetDrawable() const override;
	Processable* GetProcessable() const override;

	~MeshComponent();
};