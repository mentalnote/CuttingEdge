#include "FPSCamera.h"

FPSCamera::FPSCamera(Transform* transform, Camera* camera, std::string name) : Component(transform, name)
{
	this->camera;
	this->processable = this;
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

	float forward = 0.0f;
	float left = 0.0f;

	if (Input::GetKey(SDL_SCANCODE_W))
	{
		forward += 1.0f;
	}

	if (Input::GetKey(SDL_SCANCODE_S))
	{
		forward -= 1.0f;
	}

	if (Input::GetKey(SDL_SCANCODE_D))
	{
		left -= 1.0f;
	}

	if (Input::GetKey(SDL_SCANCODE_A))
	{
		left += 1.0f;
	}

	this->GetTransform()->SetLocalRotation(glm::rotate(this->GetTransform()->GetLocalRotation(), left * Time::GetDeltaTime() * speed, Transform::GlobalUp));
	this->GetTransform()->SetLocalPosition(this->GetTransform()->GetLocalPosition() + this->GetTransform()->GetLocalRotation() * Transform::GlobalForward * forward * Time::GetDeltaTime() * speed);
}

