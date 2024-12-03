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

		while (idx < buff.GetCount() && !AoC::CharIsLineEnd(buff[idx]) && !AoC::CharIsNumber(buff[idx]))
			idx++;

		return result;
	}
}

enum eDataType
{
	Type_ReportHeader = 0,
	Type_Level = 1,
};

struct DataPoint
{
	eDataType type;
	union
	{
		int report_count;
		int level;
	};
};

inline bool IsSafeDiff(int diff) { return (gol::Abs(diff) >= 1 && gol::Abs(diff) <= 3); }

void day2(int _day)
{
	
	gol::File file = AoC::LoadData(_day);

	char buff[32];
	const size_t file_size = file.GetSize();
	file.ReadTextLine(buff, 32);

	// start with a reasonable estimate for total file size;
	const size_t line_len = gol::StrLen(buff);
	const size_t line_data_count = (size_t)gol::Ceil<>(line_len * 0.75f);
	const size_t line_count = file_size / line_len;
	gol::Vector<DataPoint> data(line_count*line_data_count);

	do 
	{
		DataPoint& header = data.Emplace();
		header.type = Type_ReportHeader;
		header.report_count = 0;

		size_t line_count = gol::StrLen(buff);
		int idx = 0;
		while (idx < line_count && !AoC::CharIsLineEnd(buff[idx]))
		{
			int level = CharBuffToInt(buff, idx);
			DataPoint& data_point = data.Emplace();
			data_point.type = Type_Level;
			data_point.level = level;
			header.report_count++;
		}
	}
	while (file.ReadTextLine(buff, 32));
	file.Close();
	int safe_count = 0;
	
	gol::Timer timer;
	
#if PART == 1
	for (int i = 0; i < data.GetCount(); i++)
	{
		DataPoint& header = data[i];
		if (header.type == Type_ReportHeader)
		{
			bool isSafe = true;
			int prev_slope = 0;
			int report_end = i + header.report_count;
			i++;

			bool first_entry = true;
			while (i < report_end)
			{
				const DataPoint& data_point = data[i];
				const DataPoint& next_data_point = data[i+1];
				
				int diff = next_data_point.level - data_point.level;
				bool diff_is_safe = (gol::Abs(diff) >= 1 && gol::Abs(diff) <= 3);
				
				int curr_slope = gol::Sign(diff);
				bool slope_is_safe = first_entry || (prev_slope == curr_slope);

				if (!diff_is_safe || !slope_is_safe)
				{
					isSafe = false;
					break;
				}

				prev_slope = curr_slope;
				first_entry = false;
				i++;
			}

			if (isSafe)
				safe_count++;

			header.report_count = (int)isSafe;
		}
	}
#elif PART == 2
	for (int i = 0; i < data.GetCount(); i++)
	{
		DataPoint& header = data[i];
		if (header.type == Type_ReportHeader)
		{
			bool isSafe = true;
			bool dampener_active = false;
			int prev_slope = 0;
			int report_end = i + header.report_count;
			i++;

			int entry_num = 0;
			const DataPoint* prev_data_point = &data[i];
			while (i < report_end)
			{
				const DataPoint& data_point = data[i+1];
				
				// get difference between levels
				int diff = data_point.level - prev_data_point->level;
				bool diff_is_safe = IsSafeDiff(diff);
				
				// edgecase. if dampner was used in first comparison. make sure to check both
				if (!diff_is_safe && dampener_active && entry_num == (int)dampener_active)
				{
					const DataPoint& other_prev_data_point = data[i];
					int other_diff = data_point.level - other_prev_data_point.level;
					diff_is_safe = IsSafeDiff(other_diff);
					if (diff_is_safe)
					{
						diff = other_diff;
						prev_data_point = &other_prev_data_point;
					}
				}

				// get slope of levels. first valid slope is always allowed*
				int curr_slope = gol::Sign(diff);
				bool slope_is_safe = entry_num==(int)dampener_active || (prev_slope == curr_slope);

				// gross hack....
				// * check for edge case where first entry should be dampened
				if (entry_num == 1 && !dampener_active && !slope_is_safe && i < report_end-1 && diff != 0)
				{
					// check in case we need to eliminate the first level
					const DataPoint& next_data_point = data[i+2];
					int next_diff = next_data_point.level - data_point.level;
					bool next_diff_is_safe = IsSafeDiff(next_diff);

					int next_slope = gol::Sign(next_diff);
					slope_is_safe = (next_slope == curr_slope) && next_diff_is_safe;
					
					// if current and next are false, check previous and next
					if (!slope_is_safe)
					{
						next_diff = next_data_point.level - prev_data_point->level;
						next_diff_is_safe = IsSafeDiff(next_diff);

						int next_slope = gol::Sign(next_diff);
						slope_is_safe = (next_slope == prev_slope) && next_diff_is_safe;
						if (slope_is_safe)
							curr_slope = next_slope;
					}
					else if (!diff_is_safe)
					{
						// check current against first entry since current and previous isnt safe diff wise
						const DataPoint& first_data_point = data[i-1];
						int first_diff = data_point.level - first_data_point.level;
						diff_is_safe = IsSafeDiff(first_diff);
					}
					dampener_active = true;
				}

				bool unsafe = !diff_is_safe || !slope_is_safe;
				if (unsafe && dampener_active)
				{
					isSafe = false;
					break;
				}
				else if (unsafe && !dampener_active)
				{
					dampener_active = true;
				}
				else
				{
					prev_data_point = &data_point;
					prev_slope = curr_slope;
				}
				
				entry_num++;;
				i++;
			}

			if (isSafe)
				safe_count++;

			header.report_count = (int)isSafe;
		}
	}
#endif

	/*int recordIdx = 0;
	for (int i = 0; i < data.GetCount(); i++)
	{
		if (data[i].type == Type_ReportHeader)
		{
			if(i < 10)
				printf("\nreport[   %i] { %s } : ", recordIdx, data[i].report_count ? "SAFE  " : "UNSAFE");
			else if (i < 100)
				printf("\nreport[  %i] { %s } : ", recordIdx, data[i].report_count ? "SAFE  " : "UNSAFE");
			else if (i < 1000)
				printf("\nreport[ %i] { %s } : ", recordIdx, data[i].report_count ? "SAFE  " : "UNSAFE");
			else
				printf("\nreport[%i] { %s } : ", recordIdx, data[i].report_count ? "SAFE  " : "UNSAFE");

			recordIdx++;
		}
		else
		{
			printf("%i ", data[i].level);
		}
		
	}*/
	printf("\nsafe count = %i \n", safe_count);
}