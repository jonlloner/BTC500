#pragma once

#include <chrono>
#include <iostream>
#include <string>

#include "base58.h"
#include "ripemd160.h"
#include "sha256.h"

#define ITERATIONS 0
#define TEST(name, result, call) if (result) { std::cout << "[I] Test is passed: " << name; Timer timer; if (ITERATIONS) { for (int i = 0; i < ITERATIONS; i++) call; std::cout << " [" << timer.stop(ITERATIONS) << " ns]"; } std::cout << std::endl; } else { std::cout << "[I] Test is failed: " << name << std::endl; return; }

struct Timer
{
	static unsigned long long time();

	unsigned long long timer;

	Timer();
	unsigned long long stop(int iterations);
	double stop();
};

void test();
