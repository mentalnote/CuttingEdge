#include "Component.h"

Component::Component(Transform* transform, std::string name)
{
	this->transform = transform;
}

Transform* Component::GetTransform() const
{
	return this->transform;
}
