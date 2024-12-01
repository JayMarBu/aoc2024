#ifndef AOC2024_UTILS_H
#define AOC2024_UTILS_H

#include "gol/FileSystem.h"

#define CHAR_TO_INT(c) (c - 48)

constexpr bool TEST_DATA = true;

namespace AoC
{
	inline bool CharIsNumber(char c) { return c >= '0' && c <= '9'; }
	inline bool CharIsAlphabet(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
	inline bool CharIsLineEnd(char c) { return c == '\n' || c == '\r'; }

	gol::File LoadData(int day, bool testData = false)
	{
		char buff[128];
		gol::StrFmt(buff, 128, "res/day%i/%s", day, testData ? "test_data.txt" : "data.txt");

		return gol::File(buff);
	}
}

#endif //AOC2024_UTILS_H