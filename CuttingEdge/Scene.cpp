#include "Scene.h"

Scene::Scene()
{
	this->rootTransforms = std::vector<Transform*>();
	this->drawables = std::unordered_set<Drawable*>();
	this->processables = std::unordered_set<Processable*>();
	this->activeCamera = nullptr;
}

void Scene::Process()
{
	for (Processable* processable : this->processables) {
		processable->Process();
	}
}

void Scene::Draw()
{
	for (Drawable* drawable : this->drawables) {
		drawable->Draw(this->activeCamera);
	}
}

Camera * Scene::GetActiveCamera() const
{
	return this->activeCamera;
}

void Scene::SetActiveCamera(Camera * camera)
{
	this->activeCamera = camera;
}

Transform * Scene::CreateTransform(Transform * parent, std::string name)
{
	Transform* newTransform = new Transform(this, parent, name);

	if (!parent) {
		rootTransforms.push_back(newTransform);
	}

	return newTransform;
}

void Scene::AddComponent(Component * component)
{
	Drawable* drawable = component->GetDrawable();

	if (drawable) {
		this->drawables.insert(drawable);
	}

	Processable* processable = component->GetProcessable();

	if (processable) {
		this->processables.insert(processable);
	}
}

template<typename C>
C* Scene::CreateComponent(Transform * transform, std::string name)
{
	if (!transform) {
		return nullptr;
	}

	C* newComponent = new C(transform, name);

	Drawable* drawable = newComponent->GetDrawable();

	if (drawable) {
		this->drawables.insert(drawable);
	}

	Processable* processable = newComponent->GetProcessable();

	if (processable) {
		this->processables.insert(processable);
	}

	return newComponent;
}

void Scene::DeleteTransform(Transform * transform)
{
	if (!transform->GetParent()) {
		this->rootTransforms.erase(std::find(this->rootTransforms.begin(), this->rootTransforms.end(), transform));
	}

	for (Component* component : transform->FindAllComponents(Transform::MAXDEPTH)) {
		DeleteComponent(component);
	}

	delete transform;
}

void Scene::DeleteComponent(Component * component)
{
	Drawable* drawable = component->GetDrawable();

	if (drawable) {
		this->drawables.erase(std::find(this->drawables.begin(), this->drawables.end(), drawable));
	}

	Processable* processable = component->GetProcessable();

	if (processable) {
		this->processables.erase(std::find(this->processables.begin(), this->processables.end(), processable));
	}

	component->GetTransform()->_RemoveComponent(component);

	delete component;
}
