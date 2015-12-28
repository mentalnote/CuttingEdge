#pragma once
#include <chrono>
#include <string>
#include <iostream>

class Time {
private:
	static float deltaTime;
	static long long currentTime;

	static std::string flag;
	static long long flagTime;

	static Time* instance;

public:
	Time();

	void Tick();

	float static GetDeltaTime();

	void static LogFlag(std::string flag);
};