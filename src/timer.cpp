#include "timer.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <stdio.h>

namespace gol
{
	Timer::Timer()
	{
		QueryPerformanceCounter((LARGE_INTEGER *)&start);
	}
	
	Timer::~Timer()
	{
		gol::s64 end, freq;
		QueryPerformanceCounter((LARGE_INTEGER *)&end);
		QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
		printf(" -> %0.1f ms\n", float(end - start) / float(freq) * 1000.0f);
	}
}