#pragma once
class Processable {
public:
	//Function which will be called prior to draw calls every frame
	virtual void Process() = 0;

	virtual ~Processable() = 0 {};
};