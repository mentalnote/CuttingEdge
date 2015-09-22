#pragma once
#include <vector>
#include "Transform.h"

class Scene {
private:
	std::vector<Transform> rootTransforms;

public:
	void Draw();
};