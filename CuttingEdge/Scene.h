#pragma once
#include <vector>
#include "Transform.h"
#include <unordered_set>
#include "Component.h"
#include "Camera.h"
#include "Drawable.h"
#include "Processable.h"
#include "Material.h"

class Scene {
private:
	std::vector<Transform*> rootTransforms;
	std::unordered_set<Drawable*> drawables;
	std::unordered_set<Processable*> processables;
	Camera* activeCamera;

	glm::mat4 mvp;

public:
	Scene();

	void Process();

	void Draw();
	void Draw(Material* material);

	Camera* GetActiveCamera() const;
	void SetActiveCamera(Camera* camera);

	Transform* CreateTransform(Transform* parent = nullptr, std::string name = "transform");

	void AddComponent(Component* component);

	template <typename C>
	C* CreateComponent(Transform* transform, std::string name = "component");

	void DeleteTransform(Transform* transform);

	void DeleteComponent(Component* component);
};
