#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "RenderLayer.h"
#include "Scene.h"

class RenderTechnique {
	std::vector<RenderLayer*> layers;

	std::unordered_map<std::string, RenderLayer::TexBuffer*> initializedBuffers;

public:
	RenderTechnique();
	void InitializeLayers();
	void Render(Scene* scene);
};