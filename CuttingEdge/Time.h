#pragma once
#include <chrono>

class Time {
private:
	static float deltaTime;
	static long long currentTime;
	static Time* instance;

public:
	Time();

	void Tick();

	float static GetDeltaTime();
};