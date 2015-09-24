#pragma once
#include "Component.h"
#include "Camera.h"
#include "Time.h"
#include "Input.h"
#include "Processable.h"

class FPSCamera : public Component, Processable
{
private:
	Camera* camera;

	Processable* processable;

public:
	FPSCamera(Transform* transform, Camera* camera, std::string name = "FPSCamera");

	Drawable* GetDrawable() const override;

	Processable* GetProcessable() const override;

	void Process() override;
};