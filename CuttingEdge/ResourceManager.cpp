#include "ResourceManager.h"
#include <fstream>

Assimp::Importer* ResourceManager::importer = new Assimp::Importer();

std::unordered_map<std::string, Mesh::MeshData*> ResourceManager::meshMap = std::unordered_map<std::string, Mesh::MeshData*>();

std::unordered_map<std::string, Texture*> ResourceManager::textureMap = std::unordered_map<std::string, Texture*>();

std::unordered_map<std::string, ShaderProgram::Shader*> ResourceManager::shaderMap = std::unordered_map<std::string, ShaderProgram::Shader*>();

std::unordered_map<std::string, ShaderProgram*> ResourceManager::shaderProgramMap = std::unordered_map<std::string, ShaderProgram*>();

Mesh* ResourceManager::LoadMesh (std::string path) {
	return new Mesh(LoadMeshData(path));
}

Mesh::MeshData* ResourceManager::LoadMeshData(std::string path)
{
	Mesh::MeshData* meshData = meshMap[path];

	if (meshData == nullptr) {
		meshData = new Mesh::MeshData();

		const struct aiScene* modelData;

		modelData = importer->ReadFile(path, aiProcess_Triangulate);

		if (modelData == nullptr) {
			return nullptr;
		}	

		// TODO: Add option for flattening redundant vertices across sub meshes
		GLuint sizeCounter = 0;

		for (int i = 0; i < modelData->mNumMeshes; i++)
		{
			sizeCounter += modelData->mMeshes[i]->mNumVertices;
		}

		aiMesh* mesh = modelData->mMeshes[0];

		meshData->verticesCount = mesh->mNumVertices * 3;

		meshData->elementsCount = mesh->mNumFaces * 3;

		modelData->mMaterials[0]->mProperties[0]->mSemantic;

		meshData->elements = CreateFlatElementArray(mesh->mFaces, mesh->mNumFaces);

		meshData->vertices = CreateFlatVertexArray(mesh->mVertices, mesh->mNumVertices);

		meshData->path = path;

		meshMap[path] = meshData;
	}

	return meshMap[path];
}



bool ResourceManager::BufferMesh(Mesh::MeshData* meshData) {

	if (meshData->vao != 0 || meshData->vbo != 0 || meshData->ebo != 0) {
		return false;
	}

	// Create Vertex Array Object
	glGenVertexArrays(1, &(meshData->vao));
	glBindVertexArray(meshData->vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
	glGenBuffers(1, &(meshData->vbo));

	glBindBuffer(GL_ARRAY_BUFFER, meshData->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * meshData->vertices.size(), meshData->vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Create an element array
	glGenBuffers(1, &(meshData->ebo));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * meshData->elements.size(), meshData->elements.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Data buffered to GPU so remove it from memory
	meshData->elements.clear();
	meshData->vertices.clear();

	return true;
}

bool ResourceManager::CompileShaderProgram(ShaderProgram* program)
{
	if(shaderProgramMap[program->name] != nullptr)
	{
		return false;
	}

	program->programId = glCreateProgram();

	for(ShaderProgram::Shader* shader : program->shaders)
	{
		glAttachShader(program->programId, shader->id);
	}

	shaderProgramMap[program->name] = program;

	return true;
}

ShaderProgram* ResourceManager::GetShaderProgram(std::string name)
{
	return shaderProgramMap[name];
}

Texture* ResourceManager::LoadTexture(std::string path)
{
	return new Texture();
}

ShaderProgram::Shader* ResourceManager::LoadShaderSource(std::string path)
{
	ShaderProgram::Shader* shader = shaderMap[path];

	if (shader == nullptr)
	{
		shader = new ShaderProgram::Shader();

		std::string extension = path.substr(path.find_last_of('.') + 1);

		if (extension == ShaderProgram::VERT_EXTENSION)
		{
			shader->type = GL_VERTEX_SHADER;
		}
		else if (extension == ShaderProgram::FRAG_EXTENSION)
		{
			shader->type = GL_FRAGMENT_SHADER;
		}
		else if (extension == ShaderProgram::GEOM_EXTENSION)
		{
			shader->type = GL_GEOMETRY_SHADER;
		}

		std::ifstream sourceStream(path, std::ios::ate | std::ios::binary);

		size_t size = sourceStream.tellg();
		shader->source = new GLchar[size + 1];
		sourceStream.seekg(0);
		sourceStream.read(shader->source, size);
		shader->source[size] = '\0';

		shaderMap[path] = shader;
	}

	return shader;
}

bool ResourceManager::CompileShader(ShaderProgram::Shader* shader)
{
	ShaderProgram::Shader* storedShader = shaderMap[shader->path];

	if(storedShader == nullptr || storedShader->id)
	{
		return false;
	}

	storedShader->id = glCreateShader(shader->type);
	glShaderSource(storedShader->id, 1, &(shader->source), NULL);
	glCompileShader(storedShader->id);

	return true;
}

ShaderProgram::Shader* ResourceManager::LoadShader(std::string path)
{
	ShaderProgram::Shader* shader = LoadShaderSource(path);

	CompileShader(shader);

	return shader;
}

std::vector<GLfloat> ResourceManager::CreateFlatVertexArray(aiVector3D* vertices, unsigned int numVerts) {
	std::vector<GLfloat> flatVertices = std::vector<GLfloat>(numVerts * 3);

	for (unsigned int i = 0; i < numVerts; i++) {

		int index = i * 3;

		flatVertices[index] = vertices[i].x;
		flatVertices[index + 1] = vertices[i].y;
		flatVertices[index + 2] = vertices[i].z;
	}

	return flatVertices;
}

std::vector<GLuint> ResourceManager::CreateFlatElementArray(aiFace* elements, unsigned int numElems) {
	std::vector<GLuint> flatElements = std::vector<GLuint>(numElems * 3);

	for (unsigned int i = 0; i < numElems; i++) {

		int index = i * 3;

		flatElements[index] = elements[i].mIndices[0];
		flatElements[index + 1] = elements[i].mIndices[1];
		flatElements[index + 2] = elements[i].mIndices[2];
	}

	return flatElements;
}