#include "Input.h"

SDL_Event* Input::currentEvent = nullptr;
const Uint8* Input::keyboardState = nullptr;
Input* Input::instance = nullptr;

Input::Input(const Uint8* keyboardState)
{
	if(instance != nullptr)
	{
		throw 1;
	}

	instance = this;
	Input::keyboardState = keyboardState;
}

bool Input::GetKeyDown(int key)
{
	return 
		currentEvent != nullptr && 
		currentEvent->type == SDL_KEYDOWN &&
		currentEvent->key.keysym.sym == key;
}

bool Input::GetKeyUp(int key)
{
	return
		currentEvent != nullptr &&
		currentEvent->type == SDL_KEYUP &&
		currentEvent->key.keysym.sym == key;
}

bool Input::GetKey(int key)
{
	return 
		keyboardState != nullptr &&
		keyboardState[key];
}

void Input::SetInputEvent(SDL_Event* currentEvent)
{
	if (instance != this)
	{
		throw 1;
	}

	Input::currentEvent = currentEvent;
}
