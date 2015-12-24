#pragma once
#include <unordered_map>


class RenderTechnique;

class RenderLayer {
	RenderTechnique* technique;

	int depth;

	//std::unordered_map<>* inTargets;
	//std::unordered_map<>* outTargets;
};