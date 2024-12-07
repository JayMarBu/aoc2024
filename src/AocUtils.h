#ifndef AOC2024_UTILS_H
#define AOC2024_UTILS_H

#include "gol/FileSystem.h"

#define CHAR_TO_INT(c) (c - 48)

constexpr bool TEST_DATA = true;

using u8 = gol::u8;
using u16 = gol::u16;
using u32 = gol::u32;
using u64 = gol::u64;
						
using s8 = gol::s8;
using s16 = gol::s16;
using s32 = gol::s32;
using s64 = gol::s64;

namespace AoC
{
	inline bool CharIsNumber(char c) { return c >= '0' && c <= '9'; }
	inline bool CharIsAlphabet(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
	inline bool CharIsLineEnd(char c) { return c == '\n' || c == '\r'; }

	gol::File LoadData(int day, bool testData = false)
	{
		char buff[128];
		gol::StrFmt(buff, 128, "res/day%i", day);
		
		if(!gol::FolderExists(buff))
		{
			gol::CreateFolder(buff);
			gol::StrFmt(buff, 128, "res/day%i/%s", day, "test_data.txt");
			gol::File file = gol::File::Create(buff);
			file.Close();
			gol::StrFmt(buff, 128, "res/day%i/%s", day, "data.txt");
			file = gol::File::Create(buff);
			file.Close();
		}
		
		gol::StrFmt(buff, 128, "res/day%i/%s", day, testData ? "test_data.txt" : "data.txt");
		gol_assertf_fatal(gol::FileExists(buff), "File [%s] does not exist!", buff);
		return gol::File(buff);
	}
	
	int CharBuffToInt(gol::Slice<char> buff, int& idx)
	{
		int result = 0;
		for (; idx < buff.GetCount(); idx++)
		{
			const char c = buff[idx];
			if (!AoC::CharIsNumber(c))
				break;

			result *= 10;
			result += CHAR_TO_INT(c);;
		}

		return result;
	}
}

#endif //AOC2024_UTILS_H