#include "timer.h"

#include <stdio.h>

#include "gol/Time.h"

namespace gol
{
	Timer::Timer()
	{
		start = gol::Time::GetPlatformTimeVal();
	}
	
	Timer::~Timer()
	{
		const s64 end  = gol::Time::GetPlatformTimeVal(); 
		const s64 freq = gol::Time::GetPlatformTimeFreq();
		printf("    %0.6f ms\n", float(end - start) / float(freq) * 1000.0f);
	}
}