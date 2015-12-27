#include <stdio.h>
#include <fstream>
#include "Time.h"
#include "Input.h"

#include "gl_includes.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "FPSCamera.h"
#include "MeshComponent.h"
#include "Vertex.h"


using namespace std;

// Shader sources
const GLchar* vertexSource =
"#version 150 core\n"
"uniform mat4 MVP;"
"in vec3 position;"
"out vec4 colour;"
"void main() {"
"   gl_Position = MVP * vec4(position.xyz * 0.01, 1.0);"
"   colour = vec4(position.xyz, 1.0);"
"}";

const GLchar* fragmentSource =
"#version 150 core\n"
"in vec4 colour;"
"out vec4 outColor;"
"void main() {"
"   outColor = colour;"
"}";

const string bearPath = "../Resources/Models/bear.obj";
const string deerPath = "../Resources/Models/deer.obj";
const string vertPath = "../Resources/Shaders/simple.vert";
const string fragPath = "../Resources/Shaders/simple.frag";

ofstream logfile;

Scene* scene;

int Cleanup(SDL_GLContext context);

Scene* CreateDefaultScene();

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

	scene = CreateDefaultScene();

	if(scene == nullptr)
	{
		Cleanup(context);
		return 0;
	}

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

	matrixID = glGetUniformLocation(shaderProgram, "MVP"); ;

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	logfile << "ERROR: " << glGetError() << "\n";

	Time time = Time();
	Input input = Input(SDL_GetKeyboardState(nullptr));

	logfile << ResourceManager::LoadShaderSource(vertPath);

	SDL_Event windowEvent;
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			input.SetInputEvent(&windowEvent);

			if (windowEvent.type == SDL_QUIT) break;
			if (windowEvent.type == SDL_KEYUP &&
				windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
		} else
		{
			input.SetInputEvent(nullptr);
		}

		time.Tick();

		scene->Process();

		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		scene->Draw();

		SDL_GL_SwapWindow(window);
	}

	return Cleanup(context);
}

Scene* CreateDefaultScene() {
	Scene* defaultScene = new Scene();

	Transform* cTransform = defaultScene->CreateTransform(nullptr, "Cam Node");
	cTransform->SetLocalRotation(glm::quat(0.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
	cTransform->SetLocalPosition(glm::vec3(0.0f, 0.0f, -10.0f));
	Camera* camera = new Camera(Camera::ProjectionMode::Perspective, cTransform);
	
	defaultScene->AddComponent(new FPSCamera(cTransform, camera, "Camera"));
	defaultScene->SetActiveCamera(camera);

	Transform* stuffGroup = defaultScene->CreateTransform(nullptr, "Animals");

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 8; k++){
				if (k % 2) {
					string name = "";
					name = name.append("bear(").append(to_string(i)).append(", ").append(to_string(j).append(", ").append(to_string(k)).append(")"));
					Transform* bear = defaultScene->CreateTransform(stuffGroup, name);
					bear->SetLocalPosition(glm::vec3(i - 4, j - 4, k - 4));

					std::pair<Mesh**, int> mComponent = ResourceManager::LoadMesh(bearPath);

					if(mComponent.second)
					{
						for (int l = 0; l < mComponent.second; l++) {
							defaultScene->AddComponent(new MeshComponent(bear, mComponent.first[l], name));
						}
					} else
					{
						return nullptr;
					}
				}
				else {
					string name = "";
					name = name.append("deer(").append(to_string(i)).append(", ").append(to_string(j).append(", ").append(to_string(k)).append(")"));
					Transform* deer = defaultScene->CreateTransform(stuffGroup, name);
					deer->SetLocalPosition(glm::vec3(i - 4, j - 4, k - 4));

					std::pair<Mesh**, int> mComponent = ResourceManager::LoadMesh(deerPath);

					if (mComponent.second)
					{
						for (int l = 0; l < mComponent.second; l++) {
							defaultScene->AddComponent(new MeshComponent(deer, mComponent.first[l], name));
						}
					}
					else
					{
						return nullptr;
					}
				}
			}
		}
	}

	return defaultScene;
}

int Cleanup(SDL_GLContext context) {
	delete scene;
	logfile.close();
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}

