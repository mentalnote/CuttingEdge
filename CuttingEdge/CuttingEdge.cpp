#include <stdio.h>
#include <fstream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

using namespace std;

// Shader sources
const GLchar* vertexSource =
"#version 150 core\n"
"in vec2 position;"
"in vec3 color;"
"out vec3 Color;"
"void main() {"
"   Color = color;"
"   gl_Position = vec4(position, 0.0, 1.0);"
"}";
const GLchar* fragmentSource =
"#version 150 core\n"
"in vec3 Color;"
"out vec4 outColor;"
"void main() {"
"   outColor = vec4(Color, 1.0);"
"}";

static Assimp::Importer* importer;

const struct aiScene* modelData;

int Cleanup(SDL_GLContext context);

GLuint BufferMesh(aiMesh* mesh);

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	glewInit();

	importer = new Assimp::Importer();

	modelData = importer->ReadFile("../Resources/Models/barrel.obj", 0);

	if (modelData == nullptr) {
		return Cleanup(context);
	}

	GLuint numTris = BufferMesh(modelData->mMeshes[0]);

	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	SDL_Event windowEvent;
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
			if (windowEvent.type == SDL_KEYUP &&
				windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
		}

		// Clear the screen to black
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 120, GL_UNSIGNED_INT, 0);

		SDL_GL_SwapWindow(window);
	}

	return Cleanup(context);
}

int Cleanup(SDL_GLContext context) {
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}

GLuint BufferMesh(aiMesh* mesh) {

	int numVerts = mesh->mNumVertices;

	int numFaces = mesh->mNumFaces;

	GLuint* faces = new GLuint[numFaces * 3];

	GLfloat* vertices = new GLfloat[numVerts * 3];
	//Delete array sometime (use delete[])

	ofstream log;
	log.open("../log.txt");

	for (int i = 0; i < numVerts; i++) {

		int index = i * 3;

		vertices[index] = mesh->mVertices[i].x;
		vertices[index + 1] = mesh->mVertices[i].y;
		vertices[index + 2] = mesh->mVertices[i].z;
		
		log << vertices[index] << ", " << vertices[index + 1] << ", " << vertices[index + 1] << "\n";
	}

	for (int i = 0; i < numFaces; i++) {

		int index = i * 3;

		faces[index] = mesh->mFaces[i].mIndices[0];
		faces[index + 1] = mesh->mFaces[i].mIndices[1];
		faces[index + 2] = mesh->mFaces[i].mIndices[2];
	}

	log.close();

	//GLfloat vertices[] = {
	//	-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
	//	0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
	//	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
	//	-0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
	//};

	//GLuint faces[] = {
	//	0, 1, 2,
	//	2, 3, 0
	//};

	// Create Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create an element array
	GLuint ebo;
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

	return numFaces;
}

