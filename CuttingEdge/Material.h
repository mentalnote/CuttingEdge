#pragma once
#include "Texture.h"
#include <unordered_map>

class Shader;

class Material
{
public:
	enum class PropertyType
	{
		INT1,
		INT2,
		INT3,
		INT4,
		UINT1,
		UINT2,
		UINT3,
		UINT4,
		FLOAT1,
		FLOAT2,
		FLOAT3,
		FLOAT4,
		DOUBLE1,
		DOUBLE2,
		DOUBLE3,
		DOUBLE4,
		MAT2,
		MAT3,
		MAT4,
		MAT2X3,
		MAT3X2,
		MAT4X2,
		MAT2X4,
		MAT3X4,
		MAT4X3
	};

	struct MaterialProperty
	{
		std::string name;
		PropertyType type;
		void* value;
	};

	std::unordered_map<std::string, MaterialProperty>* materialProperties;

	std::vector<Shader*> shaders;

	Material();
};
