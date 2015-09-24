#pragma once
#include <vector>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>

class Component;

class Transform {
private:
	typedef char byte;

	const static int ALLOC = 16;

	const static int dirty_None = 0;
	const static int dirty_Position = 1 << 0;
	const static int dirty_Rotation = 1 << 1;
	const static int dirty_Scale = 1 << 2;
	const static int dirty_Matrix = 1 << 3;
	const static int dirty_All = ~0;

	std::string name;

	Transform* parent;
	std::vector<Transform*> children;
	std::vector<Component*> components;

	glm::mat4x4 worldMatrix;

	glm::vec3 localPosition;
	glm::vec3 worldPosition;
	
	glm::quat localRotation;
	glm::quat worldRotation;

	glm::vec3 localScale;
	glm::vec3 worldScale;

	byte dirtyFlags;

public:

	const glm::vec3 GlobalForward = glm::vec3(0.0f, 0.0f, 1.0f);
	const glm::vec3 GlobalUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// Will create a transform object parented to the provided transform and with 
	// default local component values
	explicit Transform(Transform* parent, std::string name = "Transform");

	// Will create a transform object parented to the provided transform and will 
	// set local component values based on provided parameters, parameter values not 
	// provided will have their corresponding components set to default values
	explicit Transform(glm::vec3 position = glm::vec3(), glm::quat rotation = glm::quat(), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), Transform* parent = nullptr, std::string name = "Transform");

	Transform* GetParent() const;

	// Set the transform's parent to the provided transform, if preserveWorld is true, 
	// local component values will be updated to keep the transform world component values consistent, 
	// if preserveWorld is false, the local component values will be kept and the world transform values 
	// will be different reflecting their relation to their new parent transform
	void SetParent(Transform* parent, bool preserveWorld = true);

	std::vector<Transform*> GetChildren() const;

	glm::vec3 GetLocalPosition() const;

	// Sets local position of transform, dirties world transform and world position fields
	void SetLocalPosition(glm::vec3 position);

	// Returns world position, first updating it if it's stored value is dirty
	glm::vec3 GetWorldPosition();

	// Sets world position, also calculating and setting the appropriate value for local position
	void SetWorldPosition(glm::vec3 position);

	glm::quat GetLocalRotation() const;

	// Sets local rotation of transform, dirties world transform and world rotation fields
	void SetLocalRotation(glm::quat rotation);

	// Returns world roatation, first updating it if it's stored value is dirty
	glm::quat GetWorldRotation();
	
	// Sets world rotation, also calculating and setting the appropriate value for local rotation
	void SetWorldRotation(glm::quat rotation);

	glm::vec3 GetLocalScale() const;
	
	// Sets local scale of transform, dirties world transform and world scale fields
	void SetLocalScale(glm::vec3 scale);

	// Returns world scale, first updating it if it's stored value is dirty
	glm::vec3 GetWorldScale();

	// Sets world scale, also calculating and setting the appropriate value for local scale
	void SetWorldScale(glm::vec3 scale);

	// Calculates and returns the forward vector for the transform
	glm::vec3 GetForward();

	// Returns world matrix, first updating it if it's stored value is dirty
	glm::mat4x4 GetWorldMatrix();

	// Removes a a given component from the transform
	void RemoveComponent(Component* component);

	static void* operator new(size_t size);

	static void operator delete (void *p);

	virtual ~Transform();

private:
	void removeChildRef(Transform* child);

	void addChildRef(Transform* child);
};