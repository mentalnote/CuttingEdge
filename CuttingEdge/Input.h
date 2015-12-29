#pragma once
#include "gl_includes.h"

class Input {
private:
	static SDL_Event* currentEvent;

	const static Uint8* keyboardState;

	static Input* instance;

public:
	explicit Input(const Uint8* keyboardState);

	static bool GetKeyDown(int key);

	static bool GetKeyUp(int key);

	static bool GetKey(int key);

	void SetInputEvent(SDL_Event* currentEvent);
};