#pragma once

#include "gl_includes.h"
#include "Mesh.h"
#include <unordered_map>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ResourceManager {
public:

	// Return new Mesh created with MeshData from meshMap, 
	// if MeshData for given path is not in meshMap, 
	// loads it in first
	static Mesh* LoadMesh(std::string path);

	// Return MeshData from meshMap,
	// if MeshData for given path is not in meshMap, 
	// loads it in first
	static Mesh::MeshData* LoadMeshData(std::string path);

	// Attempt to upload mesh data to GPU
	// if mesh data isn't buffered, then generate buffers for it, upload them to the GPU, 
	// and populate meshData fields (vbo, ebo) with buffer indices and return true
	// if data for a mesh is already buffered, do nothing and return false
	static bool BufferMesh(Mesh::MeshData* meshData);

private:
	static Assimp::Importer* importer;

	// unordered_map which stores data necessary to buffer and draw meshes
	static std::unordered_map<std::string, Mesh::MeshData*> meshMap;

	//Generate flat array for vertex buffer from array of vertex vectors
	static std::vector<GLfloat> CreateFlatVertexArray(aiVector3D* vertices, unsigned int numVerts);

	//Generate flat array for element buffer from array of element vectors
	static std::vector<GLuint> CreateFlatElementArray(aiFace* elements, unsigned int numElems);
};