#include "Transform.h"
#include "Component.h"
#include "Camera.h"

const glm::vec3 Transform::GlobalForward = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 Transform::GlobalUp = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 Transform::GlobalRight = glm::vec3(1.0f, 0.0f, 0.0f);

void * Transform::operator new(size_t size)
{
	return _aligned_malloc(size, Transform::ALLOC);
}

void Transform::operator delete(void * p)
{
	return _aligned_free(p);
}

Transform::~Transform()
{
	for (Transform* child : this->children) {
		delete child;
	}
}

Transform::Transform(Scene* scene, Transform* parent, std::string name)
{
	this->parent = parent;
	if (parent) {
		this->parent->addChildRef(this);
	}

	this->name = name;
	this->localPosition = glm::vec3();
	this->localRotation = glm::quat();
	this->localScale = glm::vec3(1.0f, 1.0f, 1.0f);
	this->dirtyFlags = dirty_All;
}

Transform::Transform(Scene* scene, glm::vec3 position, glm::quat rotation, glm::vec3 scale, Transform* parent, std::string name)
{
	this->parent = parent;
	if (parent) {
		this->parent->addChildRef(this);
	}

	this->name = name;
	this->localPosition = position;
	this->localRotation = rotation;
	this->localScale = scale;
	this->dirtyFlags = dirty_All;
}

Scene * Transform::GetScene() const
{
	return this->scene;
}

Transform * Transform::GetParent() const
{
	return this->parent;
}

void Transform::SetParent(Transform * parent, bool preserveWorld)
{
	if (preserveWorld) {
		if (parent == nullptr) {
			this->localPosition = this->GetWorldPosition();
			this->localRotation = this->GetWorldRotation();
			this->localScale = this->GetWorldScale();
		}
		else {
			this->localPosition = this->GetWorldPosition() - parent->GetWorldPosition();
			this->localRotation = this->GetWorldRotation() * glm::inverse(parent->GetWorldRotation());
			this->localScale = this->GetWorldScale() / parent->GetWorldScale();
		}	
	}
	else {
		this->dirtyFlags = dirty_All;
	}
	
	if (this->parent != nullptr) {
		this->parent->removeChildRef(this);
	}

	this->parent = parent;

	this->parent->addChildRef(this);
}

std::vector<Transform*> Transform::GetChildren() const
{
	return this->children;
}

glm::vec3 Transform::GetLocalPosition() const
{
	return this->localPosition;
}

void Transform::SetLocalPosition(glm::vec3 position)
{
	this->localPosition = position;
	this->dirtyFlags |= dirty_Position | dirty_Matrix;
}

glm::vec3 Transform::GetWorldPosition()
{
	if (!this->dirtyFlags & dirty_Position) {
		return this->worldPosition;
	}

	if (this->parent == nullptr) {
		this->worldPosition = this->GetLocalPosition();
	}
	else {
		this->worldPosition = parent->GetWorldPosition() + this->GetLocalPosition();
	}

	this->dirtyFlags &= ~dirty_Position;

	return this->worldPosition;
}

void Transform::SetWorldPosition(glm::vec3 position)
{
	if (parent == nullptr) {
		this->localPosition = position;
	}
	else {
		this->localPosition = position - parent->GetWorldPosition();
	}

	this->worldPosition = position;
	this->dirtyFlags &= ~dirty_Position;
}

glm::quat Transform::GetLocalRotation() const
{
	return this->localRotation;
}

void Transform::SetLocalRotation(glm::quat rotation)
{
	this->localRotation = rotation;
	this->dirtyFlags |= dirty_Rotation | dirty_Matrix;
}

glm::quat Transform::GetWorldRotation()
{
	if (!(this->dirtyFlags & dirty_Rotation)) {
		return this->worldRotation;
	}

	if (this->parent == nullptr) {
		this->worldRotation = this->GetLocalRotation();
	}
	else {
		this->worldRotation = parent->GetWorldRotation() * this->GetLocalRotation();
	}

	this->dirtyFlags &= ~dirty_Rotation;

	return this->worldRotation;
}

void Transform::SetWorldRotation(glm::quat rotation)
{
	if (parent == nullptr) {
		this->localRotation = rotation;
	}
	else {
		this->localRotation = rotation * inverse(parent->GetWorldRotation());
	}

	this->worldRotation = rotation;
	this->dirtyFlags &= ~dirty_Rotation;
}

glm::vec3 Transform::GetLocalScale() const
{
	return this->localScale;
}

void Transform::SetLocalScale(glm::vec3 scale)
{
	this->localScale = scale;
	this->dirtyFlags |= dirty_Scale | dirty_Matrix;
}

glm::vec3 Transform::GetWorldScale()
{
	if (!(this->dirtyFlags & dirty_Scale)) {
		return this->worldScale;
	}

	if (this->parent == nullptr) {
		this->worldScale = this->GetLocalScale();
	}
	else {
		this->worldScale = parent->GetWorldScale() * this->GetLocalScale();
	}

	this->dirtyFlags &= ~dirty_Scale;

	return this->worldScale;
}

void Transform::SetWorldScale(glm::vec3 scale)
{
	if (parent == nullptr) {
		this->localScale = scale;
	}
	else {
		glm::vec3 inverseWorldScale = parent->GetWorldScale();
		inverseWorldScale = glm::vec3(1.0f / inverseWorldScale.x, 1.0f / inverseWorldScale.y, 1.0f / inverseWorldScale.z);
		this->localScale = scale * inverseWorldScale;
	}

	this->worldScale = scale;
	this->dirtyFlags &= ~dirty_Scale;
}

glm::vec3 Transform::GetForward()
{
	glm::vec3 forward = GlobalForward * GetWorldRotation();
	
	return forward;
}

glm::mat4x4 Transform::GetWorldMatrix()
{
	if (!(this->dirtyFlags & dirty_Matrix)) {
		return this->worldMatrix;
	}

	glm::mat4 translate = glm::translate(glm::mat4(1.0f), this->GetWorldPosition());
	glm::mat4 rotate = glm::mat4_cast(this->GetWorldRotation());
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), this->GetWorldScale());
	
	this->worldMatrix = translate * rotate * scale;

	this->dirtyFlags &= ~dirty_Matrix;

	return this->worldMatrix;
}

glm::mat4 Transform::CalcMVPMatrix(Camera* camera)
{
	return camera->GetProjectionMatrix() * camera->GetViewMatrix() * this->GetWorldMatrix();
}

void Transform::_AddComponent(Component * component)
{
	this->components.push_back(component);
}

void Transform::_RemoveComponent(Component * component)
{
	this->components.erase(std::find(this->components.begin(), this->components.end(), component));
}

std::vector<Component*> Transform::FindAllComponents(int depth)
{
	std::vector<Component*> components = std::vector<Component*>();

	for (Component* component : this->components) {
		components.push_back(component);
	}

	if (depth) {
		for (Transform* transform : this->children) {
			std::vector<Component*> childComponents = transform->FindAllComponents(depth - 1);
			components.insert(components.end(), childComponents.begin(), childComponents.end());
		}
	}

	return components;
}

template<typename C>
std::vector<C*> Transform::FindComponents(int depth)
{
	std::vector<C*> components = std::vector<C*>();

	for (Component* component : this->components) {
		C* castComponent = dynamic_cast<C*>(component);

		if (castComponent != nullptr) {
			components.push_back(castComponent);
		}
	}

	if (depth) {
		for (Transform* transform : this->children) {
			std::vector<C*> childComponents = transform->FindComponents<C*>(depth - 1);
			components.insert(components.end(), childComponents.begin(), childComponents.end());
		}
	}

	return components;
}

void Transform::removeChildRef(Transform * child)
{
	this->children.erase(std::find(this->children.begin(), this->children.end(), child));
}

void Transform::addChildRef(Transform * child)
{
	this->children.push_back(child);
}
