#pragma once
#include "Camera.h"
#include "Material.h"

class Drawable {
public:
	//Function which will be called in order to draw an inheriting object every frame
	virtual void Draw(Camera* camera) = 0;

	virtual Material* GetMaterial() = 0;

	virtual ~Drawable() = 0 {};
};