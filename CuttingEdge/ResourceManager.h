#pragma once

#include "gl_includes.h"
#include "Mesh.h"
#include <unordered_map>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Texture.h"
#include "ShaderProgram.h"

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

	static Texture* LoadTexture(std::string path);

	static bool CompileShader(ShaderProgram::Shader* shader);

	static bool CompileShaderProgram(ShaderProgram* shader);

	static ShaderProgram* GetShaderProgram(std::string name);

	static ShaderProgram::Shader* LoadShader(std::string path);

	static ShaderProgram::Shader* LoadShaderSource(std::string path);

private:
	static Assimp::Importer* importer;

	// unordered_map which stores data necessary to buffer and draw meshes
	static std::unordered_map<std::string, Mesh::MeshData*> meshMap;

	static std::unordered_map<std::string, Texture*> textureMap;

	static std::unordered_map<std::string, ShaderProgram::Shader*> shaderMap;

	static std::unordered_map<std::string, ShaderProgram*> shaderProgramMap;

	//Generate flat array for vertex buffer from array of vertex vectors
	static std::vector<GLfloat> CreateFlatVertexArray(aiVector3D* vertices, unsigned int numVerts);

	//Generate flat array for element buffer from array of element vectors
	static std::vector<GLuint> CreateFlatElementArray(aiFace* elements, unsigned int numElems);
};