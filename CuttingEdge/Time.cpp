#include "Time.h"

float Time::deltaTime = 0.0f;
long long Time::currentTime = 0;
Time* Time::instance = nullptr;

std::unordered_map<std::string, long long> Time::flags = std::unordered_map<std::string, long long>();

Time::Time()
{
	if(instance != nullptr)
	{
		throw 1;
	}

	instance = this;

	currentTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

void Time::Tick()
{
	if (instance != this)
	{
		throw 1;
	}

	long long newTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	deltaTime = (newTime - currentTime) * 0.000001f;

	currentTime = newTime;
}

float Time::GetDeltaTime()
{
	return deltaTime;
}

float Time::Flag(std::string flag)
{
	long long cTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

	if(!flags.count(flag))
	{
		flags[flag] = cTime;
		return 0.0f;
	}

	float dTime = (cTime - flags[flag]) * 0.000001f;
	flags[flag] = cTime;

	return dTime;
}