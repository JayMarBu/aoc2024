#include "AocUtils.h"

#include "gol/FileSystem.h"
#include "gol/Vector.h"
#include "gol/Algorithms.h"
#include "timer.h"
#include <stdio.h>

#define PART 2

namespace
{
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

void day1(int _day)
{
	gol::File file = AoC::LoadData(_day);

	char buff[32];
	file.ReadTextLine(buff, 32);
	const size_t line_count = gol::StrLen(buff);
	const size_t file_size = file.GetSize();
	gol::Vector<int> column_a(file_size / line_count);
	gol::Vector<int> column_b(file_size / line_count);

	do 
	{
		int idx = 0;
		int loc_id_a = CharBuffToInt(buff, idx);

		while (!AoC::CharIsNumber(buff[idx]))
			idx++;
		
		int loc_id_b = CharBuffToInt(buff, idx);

		column_a.Push(loc_id_a);
		column_b.Push(loc_id_b);
	}
	while (file.ReadTextLine(buff, 32));
	file.Close();
	
	int total = 0;
	int similarity_score = 0;
	{
		gol::Timer timer;
		gol::Sort<int>(column_a);
		gol::Sort<int>(column_b);

#if PART == 1
		for (int i = 0; i < column_a.GetCount(); i++)
		{
			total += gol::SubtractSmallestFromLargest(column_a[i], column_b[i]);
		}
#elif PART == 2
		int last_recorded = -1;
		int last_idx = 0;
		int occurences = 0;
	
		//gol::Timer timer;
		for (int i = 0; i < column_a.GetCount(); i++)
		{
			int a = column_a[i];
			if (a == last_recorded)
				similarity_score += a * occurences;
			occurences = 0;
			for (int j = last_idx; j < column_b.GetCount(); j++)
			{
				if (a != column_b[j])
					continue;
				if (column_b[j] > a)
					last_idx = j;
				occurences++;
			}
			last_recorded = a;
			similarity_score += a * occurences;
		}
	}
	
#endif

#if PART == 1
	printf("total = %i\n", total);
#elif PART == 2
	printf("similarity_score = %i\n", similarity_score);
#endif
}
