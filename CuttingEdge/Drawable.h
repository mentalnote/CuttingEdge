#pragma once
#include "Material.h"

class Drawable {
public:
	//Function which will be called in order to draw an inheriting object every frame
	virtual void Draw() = 0;

	virtual Material* GetMaterial() = 0;

	// Internal function only for setting reference to component which owns this object
	virtual void _SetComponent(Component* component) = 0;

	virtual Component* GetComponent() = 0;

	virtual ~Drawable() = 0 {};
};