#include "FPSCamera.h"

FPSCamera::FPSCamera(Transform* transform, Camera* camera, std::string name) : Component(transform, name)
{
	this->camera;
	this->processable = (Processable*)this;
}

Drawable* FPSCamera::GetDrawable() const
{
	return nullptr;
}

Processable* FPSCamera::GetProcessable() const
{
	return this->processable;
}

void FPSCamera::Process()
{
	const float speed = 4.0f;

	int forward = 0;
	int right = 0;

	if (Input::GetKey(SDL_SCANCODE_W))
	{
		forward += 1;
	}

	if (Input::GetKey(SDL_SCANCODE_S))
	{
		forward -= 1;
	}

	if (Input::GetKey(SDL_SCANCODE_D))
	{
		right += 1;
	}

	if (Input::GetKey(SDL_SCANCODE_A))
	{
		right -= 1;
	}

	this->GetTransform()->SetLocalRotation(glm::rotate(this->GetTransform()->GetLocalRotation(), -right * Time::GetDeltaTime() * speed, this->GetTransform()->GlobalUp));
	this->GetTransform()->SetLocalPosition(this->GetTransform()->GetLocalPosition() + this->GetTransform()->GetLocalRotation() * glm::vec3(0.0f, 0.0f, -forward) * Time::GetDeltaTime() * speed);
}

