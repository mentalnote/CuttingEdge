#pragma once
#include "Component.h"
#include "Mesh.h"
#include "ResourceManager.h"

class MeshComponent : public Component {
private:
	Mesh* mesh;

public:
	MeshComponent(Transform* transform, Mesh* mesh, std::string name = "MeshComponent");

	Drawable* GetDrawable() const override;
	Processable* GetProcessable() const override;

	~MeshComponent();
};