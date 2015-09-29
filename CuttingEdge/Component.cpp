#include "Component.h"

Component::Component(Transform* transform, std::string name)
{
	this->transform = transform;
	this->transform->_AddComponent(this);
}

Transform* Component::GetTransform() const
{
	return this->transform;
}
