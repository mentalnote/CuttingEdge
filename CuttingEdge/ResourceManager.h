#pragma once
#include "Mesh.h"
#include <unordered_map>
#include <string>

class ResourceManager {

public:
	static std::unordered_map<std::string, Mesh::MeshData> meshMap;

	static Mesh LoadMesh(std::string path);
	static void BufferMesh(Mesh& mesh);
};