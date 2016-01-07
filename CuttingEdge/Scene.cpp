#include "Scene.h"
#include <iostream>
#include "Time.h"

Scene::Scene() : rootTransforms(), drawables(), processables()
{
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
	glm::mat4 viewProj = this->activeCamera->GetViewProjectionMatrix();
	std::string mvp_string = "MVP";

	for (Drawable* drawable : this->drawables) {
		Material* material = drawable->GetMaterial();
		mvp = drawable->GetComponent()->GetTransform()->CalcMVPMatrix(&viewProj);

		material->Bind();
		material->SetMatrix4(&mvp_string, &mvp);
		material->UpdateAllUniforms();
//		this->activeCamera->GetProjectionMatrix() * this->activeCamera->GetViewMatrix() * drawable->GetComponent()->GetTransform()->GetWorldMatrix();
//		glUniformMatrix4fv(0, 1, GL_FALSE, &(this->activeCamera->GetProjectionMatrix() * this->activeCamera->GetViewMatrix() * drawable->GetComponent()->GetTransform()->GetWorldMatrix())[0][0]);
//		glUniformMatrix4fv(0, 1, GL_FALSE, &drawable->GetComponent()->GetTransform()->CalcMVPMatrix(this->activeCamera)[0][0]);
//		glUniformMatrix4fv(material->shader->propertyMap["MVP"].first, 1, GL_FALSE, &mvp[0][0]);
//		glUniformMatrix4fv(0, 1, GL_FALSE, &mvp[0][0]);

		drawable->Draw();
	}
}

void Scene::Draw(Material* material)
{
	glm::mat4 viewProj = this->activeCamera->GetViewProjectionMatrix();
	std::string mvp_string = "MVP";
	material->Bind();

	for (Drawable* drawable : this->drawables) {

		glm::mat4 mvp = drawable->GetComponent()->GetTransform()->CalcMVPMatrix(&viewProj);
		material->SetMatrix4(&mvp_string, &mvp);
		material->UpdateAllUniforms();

		drawable->Draw();
	}
	
	material->Unbind();
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
