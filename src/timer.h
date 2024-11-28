#ifndef GOL_TESTBED_TIMER_H
#define GOL_TESTBED_TIMER_H

#include "gol/detail/Core.h"

namespace gol
{
	class Timer
	{
	public:
		gol::s64 start;
		Timer();
		~Timer();
	};
}

#endif //GOL_TESTBED_TIMER_H