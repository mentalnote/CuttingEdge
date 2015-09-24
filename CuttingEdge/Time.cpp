#include "Time.h"

float Time::deltaTime = 0.0f;
long long Time::currentTime = 0;
Time* Time::instance = nullptr;

Time::Time()
{
	if(instance != nullptr)
	{
		throw 1;
	}

	instance = this;

	currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Time::Tick()
{
	if (instance != this)
	{
		throw 1;
	}

	long long newTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	deltaTime = (newTime - currentTime) / 1000.0f;

	currentTime = newTime;
}

float Time::GetDeltaTime()
{
	return deltaTime;
}