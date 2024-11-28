#ifndef AOC2023_UTILS_H
#define AOC2023_UTILS_H

#include <stdio.h>
#include <conio.h>

#define CHAR_TO_INT(c) (c - 48)

namespace AoC
{
	inline bool CharIsNumber(char c) { return c >= '0' && c <= '9'; }
	inline bool CharIsAlphabet(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
	inline bool CharIsLineEnd(char c) { return c == '\n' || c == '\r'; }
	void Wait() { while(1){ if(_kbhit()){break;}} }
}

#endif //AOC2023_UTILS_H