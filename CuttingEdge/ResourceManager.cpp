#include "ResourceManager.h"

Assimp::Importer* ResourceManager::importer = new Assimp::Importer();

std::unordered_map<std::string, Mesh::MeshData*> ResourceManager::meshMap = std::unordered_map<std::string, Mesh::MeshData*>();

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

		aiMesh* mesh = modelData->mMeshes[0];

		meshData->verticesCount = mesh->mNumVertices * 3;

		meshData->elementsCount = mesh->mNumFaces * 3;

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

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