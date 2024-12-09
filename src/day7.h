#include "AocUtils.h"

#include "gol/Vec.h"
#include "gol/Vector.h"
#include "gol/StringAlgorithms.h"
#include "timer.h"
#include <stdio.h>

#define PART 1

struct Data
{
	s32 count;
	u64 value;
};

u64 Compute(gol::Slice<Data> data)
{
	gol::Timer timer;
	u64 total = 0;
	gol::Vector<u64> permutations;
	
#if PART == 1
	int operator_count = 2;
#elif PART == 2
	int operator_count = 3;
#endif
	
	for(int i = 0; i < data.GetCount();)
	{
		Data& result = data[i];
		
		i++;
		size_t end_idx = i + result.count;
		
		permutations.Reserve(result.count*result.count);
		permutations.Push(data[i].value);
		int layer = 1;
		int prev_layer_size = 1;
		
		i++;
		for(; i < end_idx; i++)
		{
			for(int j = 0; j < prev_layer_size; j++)
			{
				size_t idx = layer + (j*operator_count);
				idx = (idx - 1) / operator_count;
				
				u64 lhs = permutations[idx];
				u64 rhs = data[i].value;
				
				permutations.Push(lhs*rhs);
				permutations.Push(lhs+rhs);
				
#if PART == 2
				permutations.Push(AoC::ConcatinateInts(lhs,rhs));
#endif
			}
			
			prev_layer_size *= operator_count;
			layer = (layer * operator_count) + 1;
		}
		
		for(int j = (layer - 1) / operator_count; j < permutations.GetCount(); j++)
		{
			if(permutations[j] == result.value)
			{
				total += result.value;
				break;
			}
		}
		
		permutations.Clear();
	}
	return total;
}

void day7(int _day)
{
	gol::File file = AoC::LoadData(_day);
	
	gol::Vector<Data> data(file.GetSize() / 2);
	
	const size_t buff_len = 128;
	char buff[buff_len];	
	while (file.ReadTextLine(buff, buff_len))
	{
		int i = 0;
		
		size_t result_idx = data.GetCount();
		Data& result = data.Emplace();
		result.count = 0;
		result.value = AoC::CharBuffToInt<u64>(buff, i);
		
		size_t str_len = gol::StrLen(buff);
		while(i < str_len && !AoC::CharIsLineEnd(buff[i]))
		{
			while(!AoC::CharIsNumber(buff[i]))
				i++;
			
			Data& input = data.Emplace();
			input.value = AoC::CharBuffToInt<u64>(buff, i);
			i++;
			data[result_idx].count++;
		}
	}
	
	u64 total = Compute(data);
	
	printf("\ntotal = %llu\n", total);
}