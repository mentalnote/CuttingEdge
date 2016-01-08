#include <stdio.h>
#include <fstream>
#include <iostream>

#include "Time.h"
#include "Input.h"

#include "gl_includes.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "FPSCamera.h"
#include "MeshComponent.h"
#include "Texture.h"
#include "guicon.h"
#include "Primitives.h"

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
const string bearTexPath = "../Resources/Textures/bear.png";
const string deerPath = "../Resources/Models/deer.obj";
const string deerTexPath = "../Resources/Textures/deer.png";
const string vertPath = "../Resources/Shaders/simple.vert";
const string fragPath = "../Resources/Shaders/simple.frag";
const string SIMPLE_SHADER = "simple";
const string MAIN_TEX = "mainTex";

ofstream logfile;

Scene* scene;

int Cleanup(SDL_GLContext context);

Scene* CreateDefaultScene();

int main(int argc, char *argv[])
{
	logfile.open("../log.txt");

	RedirectIOToConsole();

	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 1200, 800, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;

	glewInit();

	SDL_GL_SetSwapInterval(0);

	glFrontFace(GL_CW);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	ShaderProgram* program = new ShaderProgram(SIMPLE_SHADER);

	program->shaders.push_back(ResourceManager::LoadShader(vertPath));
	program->shaders.push_back(ResourceManager::LoadShader(fragPath));

	ResourceManager::CompileShaderProgram(program);

	scene = CreateDefaultScene();

	if(scene == nullptr)
	{
		Cleanup(context);
		return 0;
	}

	logfile << "ERROR: " << glGetError() << "\n";

	Material testMat = Material(ResourceManager::GetShaderProgram(SIMPLE_SHADER));

	for (unsigned int i = 0; i < testMat.shader->properties.size(); i++)
	{
		auto prop = testMat.shader->properties[i];
		logfile << prop.first << ": " << i << ", " << (int)prop.second << "\n";
	}

	double frameCount = 0;
	double totalTime = 0;

	Time time = Time();
	Input input = Input(SDL_GetKeyboardState(nullptr));

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
		
		cout << to_string(Time::GetDeltaTime()) << "\n";
		frameCount++;
		totalTime += Time::GetDeltaTime();
		time.Tick();
		scene->Process();

		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scene->Draw();

		SDL_GL_SwapWindow(window);
	}

	logfile << "Average frame latency: " << totalTime / frameCount << "\n";
	logfile << "Average frame rate: " << frameCount / totalTime << "\n";

	return Cleanup(context);
}

Scene* CreateDefaultScene() {
	Scene* defaultScene = new Scene();

	Texture* bearTex = ResourceManager::LoadTexture(bearTexPath);
	ResourceManager::BufferTexture(bearTex->data);

	Texture* deerTex = ResourceManager::LoadTexture(deerTexPath);
	ResourceManager::BufferTexture(deerTex->data);

	Transform* cTransform = defaultScene->CreateTransform(nullptr, "Cam Node");
	cTransform->SetLocalRotation(glm::quat(0.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
	cTransform->SetLocalPosition(glm::vec3(0.0f, 0.0f, -10.0f));
	Camera* camera = new Camera(Camera::ProjectionMode::Perspective, cTransform);
	
	defaultScene->AddComponent(new FPSCamera(cTransform, camera, "Camera"));
	defaultScene->SetActiveCamera(camera);

	Transform* stuffGroup = defaultScene->CreateTransform(nullptr, "Animals");

	int cSize = 8;

//	Mesh* quad = Primitives::CreateQuad();
//
//	Transform* quadTransform = defaultScene->CreateTransform(stuffGroup, "quad transform");
//	quadTransform->SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
//	quadTransform->SetLocalRotation(glm::quat(glm::vec3(glm::radians(270.0f), 0.0f, 0.0f)));
//	quadTransform->SetLocalScale(glm::vec3(1.0f, 1.0f, 1.0f));
//
//	MeshComponent* quadComponent = new MeshComponent(quadTransform, quad, "quad");
//	quadComponent->SetMaterial(new Material(ResourceManager::GetShaderProgram(SIMPLE_SHADER)));
//	quadComponent->GetMaterial()->SetTexture(MAIN_TEX, bearTex);
//	defaultScene->AddComponent(quadComponent);

	for (int i = 0; i < cSize; i++) {
		for (int j = 0; j < cSize; j++) {
			for (int k = 0; k < cSize; k++){
				if (k % 2 == 0) {
					string name = "";
					name = name.append("bear(").append(to_string(i)).append(", ").append(to_string(j).append(", ").append(to_string(k)).append(")"));
					Transform* bear = defaultScene->CreateTransform(stuffGroup, name);
					bear->SetLocalPosition(glm::vec3(i - cSize * 0.5, j - cSize * 0.5, k - cSize * 0.5));
					bear->SetLocalScale(glm::vec3(0.01f, 0.01f, 0.01f));

					std::pair<Mesh**, int> mComponent = ResourceManager::LoadMesh(bearPath);

					if(mComponent.second)
					{
						for (int l = 0; l < mComponent.second; l++) {
							MeshComponent* meshComponent = new MeshComponent(bear, mComponent.first[l], name);
							meshComponent->SetMaterial(new Material(ResourceManager::GetShaderProgram(SIMPLE_SHADER)));
							meshComponent->GetMaterial()->SetTexture(MAIN_TEX, bearTex);
							defaultScene->AddComponent(meshComponent);
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
					deer->SetLocalPosition(glm::vec3(i - cSize * 0.5, j - cSize * 0.5, k - cSize * 0.5));
					deer->SetLocalScale(glm::vec3(0.01f, 0.01f, 0.01f));

					std::pair<Mesh**, int> mComponent = ResourceManager::LoadMesh(deerPath);

					if (mComponent.second)
					{
						for (int l = 0; l < mComponent.second; l++) {
							MeshComponent* meshComponent = new MeshComponent(deer, mComponent.first[l], name);
							meshComponent->SetMaterial(new Material(ResourceManager::GetShaderProgram(SIMPLE_SHADER)));
							meshComponent->GetMaterial()->SetTexture(MAIN_TEX, deerTex);
							defaultScene->AddComponent(meshComponent);
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

