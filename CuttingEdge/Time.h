#pragma once
#include <chrono>
#include <string>
#include <iostream>
#include <unordered_map>

class Time {
private:
	static float deltaTime;
	static long long currentTime;

	static std::unordered_map<std::string, long long> flags;

	static Time* instance;

public:
	Time();

	void Tick();

	float static GetDeltaTime();

	float static Flag(std::string flag);
};