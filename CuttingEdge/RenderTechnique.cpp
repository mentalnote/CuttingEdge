#include "RenderTechnique.h"

RenderTechnique::RenderTechnique() : layers()
{
//	this->layers = std::vector<RenderLayer*>();
}

void RenderTechnique::InitializeLayers()
{
	this->initializedBuffers = std::unordered_map<std::string, RenderLayer::TexBuffer*>();

	for (unsigned int i = 0; i < this->layers.size(); i++)
	{
		std::vector<RenderLayer::TexBuffer*> inputReqs = this->layers[i]->GetRequiredInputs();

		std::vector<RenderLayer::TexBuffer*> inputs = std::vector<RenderLayer::TexBuffer*>();

		for (unsigned int j = 0; j < inputReqs.size(); j++)
		{
			 RenderLayer::TexBuffer* reqBuffer = this->initializedBuffers.at(inputReqs[j]->name);

			 inputs.push_back(reqBuffer);
		}

		this->layers[i]->Initialize(inputs);

		std::vector<RenderLayer::TexBuffer*> outputs = this->layers[i]->GetOutputBuffers();

		for (unsigned int j = 0; j < outputs.size(); j++)
		{
			this->initializedBuffers[outputs[j]->name] = outputs[j];
		}
	}
}

void RenderTechnique::Render(Scene* scene)
{
	for (unsigned int i = 0; i < this->layers.size(); i++)
	{
		this->layers[i]->Render(scene);
	}
}