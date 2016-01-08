#include "RenderLayer.h"

void RenderLayer::Initialize(std::vector<TexBuffer*> inputBuffers)
{
	this->inputBuffers = inputBuffers;

	if(this->outputBuffers.size() < 1)
	{
		return;
	}

	glGenFramebuffers(1, &this->frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);

	for (unsigned int i = 0; i < this->outputBuffers.size(); i++)
	{
		TexBuffer* buffer = this->outputBuffers[i];

		glGenTextures(1, &buffer->Buffer);
		glBindTexture(GL_TEXTURE_2D, buffer->Buffer);

		glTexImage2D(buffer->Target, buffer->Level, buffer->InternalFormat, buffer->Width, buffer->Height, buffer->Border, buffer->Format, buffer->DataType, buffer->Data);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, buffer->Buffer, buffer->Level);
	}

	glGenRenderbuffers(1, &this->stencilBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, this->stencilBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->outputBuffers[0]->Width, this->outputBuffers[0]->Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->stencilBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::vector<RenderLayer::TexBuffer*> RenderLayer::GetRequiredInputs()
{
	return this->inputBufferReqs;
}

std::vector<RenderLayer::TexBuffer*> RenderLayer::GetOutputBuffers()
{
	return this->outputBuffers;
}