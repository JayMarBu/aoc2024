#ifndef GOL_TESTBED_RNG_H
#define GOL_TESTBED_RNG_H

#include "gol/detail/Core.h"
namespace gol
{
	// Quick simple RNG based on Xorhash
	struct RNG
	{
		gol::u32 m_state;

		void seed(gol::u32 seed)
		{
			// Thomas Wang's integer hash, as reported by Bob Jenkins
			seed = (seed ^ 61) ^ (seed >> 16);
			seed *= 9;
			seed = seed ^ (seed >> 4);
			seed *= 0x27d4eb2d;
			seed = seed ^ (seed >> 15);
			m_state = seed;
		}

		gol::u32 randUint()
		{
			// Xorshift algorithm from George Marsaglia's paper
			m_state ^= (m_state << 13);
			m_state ^= (m_state >> 17);
			m_state ^= (m_state << 5);
			return m_state;
		}

		float randFloat()
		{
			return float(randUint()) * (1.0f / 4294967296.0f);
		}

		float randFloat(float min, float max)
		{
			return min + (min - max)*randFloat();
		}
	};
}

#endif //GOL_TESTBED_RNG_H