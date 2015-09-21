#include <stdio.h>
#include <fstream>

#include "gl_includes.h"
#include "ResourceManager.h"
#include "Drawable.h"

using namespace std;

// Shader sources
const GLchar* vertexSource =
"#version 150 core\n"
"in vec3 position;"
"void main() {"
"   gl_Position = vec4(position.xy * 0.2, 0.0, 1.0);"
"}";

const GLchar* fragmentSource =
"#version 150 core\n"
"out vec4 outColor;"
"void main() {"
"   outColor = vec4(1.0, 1.0, 0.0, 1.0);"
"}";

ofstream logfile;

Drawable** drawArray;

int Cleanup(SDL_GLContext context);

int main(int argc, char *argv[])
{
	logfile.open("../log.txt");
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;

	glewInit();

	drawArray = new Drawable*[2];

	try {
		drawArray[0] = &ResourceManager::LoadMesh(std::string("../Resources/Models/barrel.obj"));
		drawArray[1] = &ResourceManager::LoadMesh(std::string("../Resources/Models/pot.obj"));
	}
	catch (int e) {
		logfile << "ERROR: " << e << " Failed to open file/s" << "\n";
	}

	ResourceManager::BufferMesh(((Mesh*) drawArray[0])->GetMeshData());
	ResourceManager::BufferMesh(((Mesh*) drawArray[1])->GetMeshData());

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

	logfile << "ERROR: " << glGetError() << "\n";

	SDL_Event windowEvent;
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
			if (windowEvent.type == SDL_KEYUP &&
				windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
		}

		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i < 2; i++) {
			drawArray[i]->Draw();
		}

		SDL_GL_SwapWindow(window);
	}

	return Cleanup(context);
}

int Cleanup(SDL_GLContext context) {
	logfile.close();
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}

