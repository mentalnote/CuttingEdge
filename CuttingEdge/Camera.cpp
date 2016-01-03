#include "Camera.h"

void * Camera::operator new(size_t size)
{
	return _aligned_malloc(size, Camera::ALLOC);
}

void Camera::operator delete(void * p)
{
	return _aligned_free(p);
}

Camera::Camera(ProjectionMode projectionMode, Transform* transform, int width, int height)
{
	this->projectionMode = projectionMode;

	this->transform = transform;
	this->transform->SetCacheInverseMatrix(true);

	this->left = 0.0f;
	this->right = width;
	this->top = 0.0f;
	this->bottom = height;

	this->aspect = (float)width / (float)height;
	this->fov = 90.0f;

	this->nearClip = 0.1f;
	this->farClip = 1000.0f;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	if(this->isProjDirty)
	{
		if(this->projectionMode == Orthographic)
		{
			this->projectionMatrix = glm::ortho(this->left, this->right, this->bottom, this->top, this->nearClip, this->farClip);
		} else
		{
			this->projectionMatrix = glm::perspective(this->fov, this->aspect, this->nearClip, this->farClip);
		}

		this->isProjDirty = false;
	}
	
	return this->projectionMatrix;
}

glm::mat4 Camera::GetViewMatrix()
{
	return this->transform->GetInverseWorldMatrix();
}

glm::mat4 Camera::GetViewProjectionMatrix()
{
	return this->GetProjectionMatrix() * this->GetViewMatrix();
}

Transform * Camera::GetTransform()
{
	return this->transform;
}

Camera::ProjectionMode Camera::GetProjectionMode() const
{
	return this->projectionMode;
}

void Camera::SetProjectionMode(ProjectionMode projectionMode)
{
	this->projectionMode = projectionMode;
	this->isProjDirty = true;
}

float Camera::GetLeft() const
{
	return this->left;
}

void Camera::SetLeft(float left)
{
	this->left = left;
	this->isProjDirty = true;
}

float Camera::GetRight() const
{
	return this->right;
}

void Camera::SetRight(float right)
{
	this->right = right;
	this->isProjDirty = true;
}

float Camera::GetTop() const
{
	return this->top;
}

void Camera::SetTop(float top)
{
	this->top = top;
	this->isProjDirty = true;
}

float Camera::GetBottom() const
{
	return this->bottom;
}

void Camera::SetBottom(float bottom)
{
	this->bottom = bottom;
	this->isProjDirty = true;
}

float Camera::GetNear() const
{
	return this->nearClip;
}

void Camera::SetNear(float near)
{
	this->nearClip;
	this->isProjDirty = true;
}

float Camera::GetFar() const
{
	return this->farClip;
}

void Camera::SetFar(float far)
{
	this->farClip;
	this->isProjDirty = true;
}

float Camera::GetFOV() const
{
	return this->fov;
}

void Camera::SetFOV(float fov)
{
	this->fov = fov;
	this->isProjDirty = true;
}

float Camera::GetAspect() const
{
	return this->aspect;
}

void Camera::SetAspect(float aspect)
{
	this->aspect = aspect;
	this->isProjDirty = true;
}