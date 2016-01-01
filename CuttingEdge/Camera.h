#pragma once
#include <glm\gtc\matrix_transform.hpp>
#include "Transform.h"

class Camera {
public:
	enum ProjectionMode
	{
		Perspective,
		Orthographic
	};

	explicit Camera(ProjectionMode projectionMode, Transform* transform, int width = 800, int height = 600);

private:
	const static int ALLOC = 16;

	ProjectionMode projectionMode;

	Transform* transform;

	glm::mat4 projectionMatrix;

	bool isProjDirty;

	float left;
	float right;
	float top;
	float bottom;

	float nearClip;
	float farClip;

	float fov;
	float aspect;

public:
	glm::mat4 GetProjectionMatrix();

	glm::mat4 GetViewMatrix();

	glm::mat4 GetViewProjectionMatrix();

	Transform* GetTransform();

	ProjectionMode GetProjectionMode() const;
	void SetProjectionMode(ProjectionMode projectionMode);

	float GetLeft() const;
	void SetLeft(float left);

	float GetRight() const;
	void SetRight(float right);

	float GetTop() const;
	void SetTop(float top);

	float GetBottom() const;
	void SetBottom(float bottom);

	float GetNear() const;
	void SetNear(float nearClip);

	float GetFar() const;
	void SetFar(float farClip);

	float GetFOV() const;
	void SetFOV(float fov);

	float GetAspect() const;
	void SetAspect(float aspect);

	static void* operator new(size_t size);

	static void operator delete (void *p);
};