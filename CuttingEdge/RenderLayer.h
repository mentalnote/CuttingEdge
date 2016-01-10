#pragma once
#include <unordered_map>
#include <vector>
#include "gl_includes.h"
#include <string>
#include "Scene.h"

class RenderLayer {
public:
	struct TexBuffer
	{
		std::string name;
		GLuint Buffer;
		GLenum Target;
		GLint InternalFormat;
		GLenum Format;
		GLsizei Width;
		GLsizei Height;
		GLint Level;
		GLint Border;
		GLenum DataType;
		const GLvoid* Data;
	};

protected:
	std::vector<TexBuffer*> inputBufferReqs;
	std::vector<TexBuffer*> inputBuffers;
	std::vector<TexBuffer*> outputBuffers;
	GLuint frameBuffer;
	GLuint stencilBuffer;

public:
	virtual void Initialize(std::vector<TexBuffer*> inputBuffers);

	std::vector<TexBuffer*> GetRequiredInputs();

	std::vector<TexBuffer*> GetOutputBuffers();

	virtual void Render(Scene* scene) = 0;

	virtual ~RenderLayer() = 0 {};
};