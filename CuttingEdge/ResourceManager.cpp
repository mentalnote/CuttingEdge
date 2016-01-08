#include "ResourceManager.h"
#include <fstream>

Assimp::Importer* ResourceManager::importer = new Assimp::Importer();

std::unordered_map<std::string, std::pair<Mesh::MeshData**, int>> ResourceManager::meshMap = std::unordered_map<std::string, std::pair<Mesh::MeshData**, int>>();

std::unordered_map<std::string, Texture::TextureData*> ResourceManager::textureMap = std::unordered_map<std::string, Texture::TextureData*>();

std::unordered_map<std::string, ShaderProgram::Shader*> ResourceManager::shaderMap = std::unordered_map<std::string, ShaderProgram::Shader*>();

std::unordered_map<std::string, ShaderProgram*> ResourceManager::shaderProgramMap = std::unordered_map<std::string, ShaderProgram*>();

std::pair<Mesh**, int> ResourceManager::LoadMesh (std::string path) {
	std::pair<Mesh**, int> meshes = std::pair<Mesh**, int>();
	
	std::pair<Mesh::MeshData**, int> meshData = LoadMeshData(path);

	if (meshData.first == nullptr) {
		return meshes;
	}

	meshes.first = new Mesh*[meshData.second];
	meshes.second = meshData.second;

	for (int i = 0; i < meshData.second; i++) {
		meshes.first[i] = new Mesh(meshData.first[i]);
	}

	return meshes;
}

std::pair<Mesh::MeshData**, int> ResourceManager::LoadMeshData(std::string path)
{
	std::pair<Mesh::MeshData**, int> meshData = meshMap[path];

	if (meshData.first == nullptr) {
		meshData = std::pair<Mesh::MeshData**, int>();

		const struct aiScene* modelData;

		modelData = importer->ReadFile(path, aiProcess_Triangulate);

		if (modelData == nullptr) {
			return meshData;
		}	

		Mesh::MeshData** meshes = new Mesh::MeshData*[modelData->mNumMeshes];
		meshData.second = modelData->mNumMeshes;

		for (unsigned int i = 0; i < modelData->mNumMeshes; i++)
		{
			aiMesh* mesh = modelData->mMeshes[i];

			meshes[i] = new Mesh::MeshData();

			meshes[i]->verticesCount = mesh->mNumVertices * 3;

			meshes[i]->elementsCount = mesh->mNumFaces * 3;

			meshes[i]->elements = CreateFlatElementArray(mesh->mFaces, mesh->mNumFaces);

			meshes[i]->vertices = CreateFlatVertexArray(mesh);

			meshes[i]->name = std::string(mesh->mName.C_Str());

			meshes[i]->path = path;
		}

		meshData.first = meshes;

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
	glBufferData(GL_ARRAY_BUFFER, Vertex::SIZE * meshData->vertices.size(), (GLfloat*)meshData->vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::SIZE, (void*)Vertex::POS_OFFSET);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Vertex::SIZE, (void*)Vertex::NORMAL_OFFSET);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, Vertex::SIZE, (void*)Vertex::COLOR_OFFSET);
	
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, Vertex::SIZE, (void*)Vertex::UV_OFFSET);

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

	glBindFragDataLocation(program->programId, 0, "outColor");
	glLinkProgram(program->programId);

	GLint isLinked = 0;
	glGetProgramiv(program->programId, GL_LINK_STATUS, (int *)&isLinked);

	GLint numBlocks;
	glGetProgramiv(program->programId, GL_ACTIVE_UNIFORMS, &numBlocks);

	GLint nameLen;
	glGetProgramiv(program->programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameLen);

	std::vector<GLchar> name;
	name.resize(nameLen);

	program->properties.resize(numBlocks);

	for (int blockIx = 0; blockIx < numBlocks; ++blockIx)
	{
		GLint size;
		GLenum type;
		glGetActiveUniform(program->programId, blockIx, nameLen, NULL, &size, &type, &name[0]);

		program->properties[blockIx] = { std::string(&name[0]), type };
	}

	shaderProgramMap[program->name] = program;

	return isLinked > 0;
}

ShaderProgram* ResourceManager::GetShaderProgram(std::string name)
{
	return shaderProgramMap[name];
}

Texture* ResourceManager::LoadTexture(std::string path)
{
	Texture::TextureData* texData = textureMap[path];

	if(texData == nullptr)
	{
		texData = new Texture::TextureData();
		texData->data = stbi_load(path.c_str(), &texData->width, &texData->height, &texData->components, 0);

		if(texData->data == nullptr)
		{
			return nullptr;
		}

		texData->path = path;

		std::string name = path;
		
		int sIndex = path.find_last_of('/');
		int eIndex = path.find_last_of('.');

		texData->name = path.substr(sIndex, eIndex - sIndex);

		textureMap[path] = texData;
	}

	return new Texture(texData);
}

bool ResourceManager::BufferTexture(Texture::TextureData* texData)
{
	if(texData->index != 0)
	{
		return false;
	}

	glGenTextures(1, &texData->index);

	glBindTexture(GL_TEXTURE_2D, texData->index);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (texData->components == 3) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texData->width, texData->height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData->data);
	} else if (texData->components == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texData->width, texData->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData->data);
	}	

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

ShaderProgram::Shader* ResourceManager::LoadShaderSource(std::string path)
{
	ShaderProgram::Shader* shader = shaderMap[path];

	if (shader == nullptr)
	{
		shader = new ShaderProgram::Shader();

		shader->path = path;

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

std::vector<Vertex> ResourceManager::CreateFlatVertexArray(aiMesh* mesh) {
	std::vector<Vertex> flatVertices = std::vector<Vertex>(mesh->mNumVertices);

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D pos = mesh->mVertices[i];

		flatVertices[i].position = glm::vec3(pos.x, pos.y, pos.z);

		if (mesh->HasNormals())
		{
			aiVector3D normal = mesh->mNormals[i];
			flatVertices[i].normal = glm::vec3(normal.x, normal.y, normal.z);
		}
		if (mesh->HasVertexColors(0))
		{
			aiColor4D color = mesh->mColors[0][i];
			flatVertices[i].color = glm::vec3(color.r, color.g, color.b);
		}
		if(mesh->HasTextureCoords(0))
		{
			auto texCoords = mesh->mTextureCoords[0][i];
			flatVertices[i].uv = glm::vec2(texCoords.x, texCoords.y);
		}
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