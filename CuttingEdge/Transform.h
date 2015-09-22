#pragma once
#include <vector>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>

class Transform {
private:
	typedef char byte;

	const int dirty_None = 0;
	const int dirty_Position = 1 << 0;
	const int dirty_Rotation = 1 << 1;
	const int dirty_Scale = 1 << 2;
	const int dirty_Matrix = 1 << 3;
	const int dirty_All = ~0;

	Transform* parent;
	std::vector<Transform*> children;

	glm::mat4x4 worldMatrix;

	glm::vec3 localPosition;
	glm::vec3 worldPosition;
	
	glm::quat localRotation;
	glm::quat worldRotation;

	glm::vec3 localScale;
	glm::vec3 worldScale;

	byte dirtyFlags;

public:
	Transform* GetParent() const;
	void SetParent(Transform* parent, bool preserveWorld = true);

	std::vector<Transform*> GetChildren() const;

	glm::vec3 GetLocalPosition() const;
	void SetLocalPosition(glm::vec3 position);

	glm::vec3 GetWorldPosition();
	void SetWorldPosition(glm::vec3 position);

	glm::quat GetLocalRotation() const;
	void SetLocalRotation(glm::quat rotation);

	glm::quat GetWorldRotation();
	void SetWorldRotation(glm::quat rotation);

	glm::vec3 GetLocalScale() const;
	void SetLocalScale(glm::vec3 scale);

	glm::vec3 GetWorldScale();
	void SetWorldScale(glm::vec3 scale);

	glm::mat4x4 GetWorldMatrix();

private:
	void removeChildRef(Transform* child);
};