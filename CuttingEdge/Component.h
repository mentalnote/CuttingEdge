#pragma once
#include "string"
#include "Transform.h"
#include "Drawable.h"
#include "Processable.h"

class Component {
protected:
	Transform* transform;

	std::string name;

public :
	explicit Component(Transform* transform, std::string name = "component");

	virtual Drawable* GetDrawable() const = 0;

	virtual Processable* GetProcessable() const = 0;

	Transform* GetTransform() const;

	virtual ~Component() = 0 {};
};