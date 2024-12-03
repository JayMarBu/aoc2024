#include "AocUtils.h"

#include "gol/FileSystem.h"
#include "gol/Vector.h"
#include "gol/Algorithms.h"
#include "timer.h"
#include <stdio.h>

#define PART 2

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

gol::String g_str;
bool g_enable_state = true;

bool CheckChar(int i, char c) 
{
	bool isInRange = (i >= g_str.GetSize());
	return (isInRange || g_str[i] != c);
}

#if PART == 1
bool FindValidMulToken(int& i, int& lhs, int& rhs)
{
	int count = g_str.GetSize();
	while(i < count)
	{
		//************************
		//  find "mul(" prefix
		while(i < count && g_str[i] != 'm')
			i++;
		
		if(i >= count)
			return false;
		
		if(CheckChar(++i, 'u'))
			continue;
		if(CheckChar(++i, 'l'))
			continue;
		if(CheckChar(++i, '('))
			continue;
		
		i++;
		if(i >= count)
			return false;
		
		//************************
		//  find left hand input
		if(!AoC::CharIsNumber(g_str[i]))
			continue;
		
		int idx = 0;
		lhs = CharBuffToInt(g_str.GetSubSlice(i,GOL_MIN(4, (count-1) - i)), idx);
		if(lhs >= 1000) // number of digits > 3
			continue;
		
		i += idx;
		if(i >= count)
			return false;
		
		//************************
		//  verify valid comma
		if(CheckChar(i,','))
			continue;
		
		i++;
		if(i >= count)
			return false;
		
		//************************
		//  find right hand input
		idx = 0;
		rhs = CharBuffToInt(g_str.GetSubSlice(i,GOL_MIN(4, (count-1) - i)), idx);
		if(rhs >= 1000) // number of digits > 3
			continue;
		
		i += idx;
		if(i >= count)
			return false;
		
		//************************
		//  find valid parenthisis
		if(CheckChar(i,')'))
			continue;
		
		// yippee!! we found a valid mul(xxx,yyy) token!
		return true;
	}
	
	return false;
}
#elif PART == 2
bool FindValidMulToken(int& i, int& lhs, int& rhs)
{
	int count = g_str.GetSize();
	while(i < count)
	{
		//************************
		//  find possible token prefix
		while(i < count && (g_str[i] != 'm' && g_str[i] != 'd'))
			i++;
		
		if(i >= count)
			return false;
		
		//************************
		//  we found a possible do/don't
		if(g_str[i] == 'd')
		{
			if(CheckChar(++i, 'o'))
				continue;
			
			bool possible_dont = !CheckChar(i+1, 'n');
			bool possible_do = !CheckChar(i+1, '(');
			
			i++;
			if(possible_do)
			{
				if(CheckChar(++i, ')'))
					continue;
				
				g_enable_state = true;
			}
			else if(possible_dont)
			{
				if(CheckChar(++i, '\''))
					continue;
				if(CheckChar(++i, 't'))
					continue;
				if(CheckChar(++i, '('))
					continue;
				if(CheckChar(++i, ')'))
					continue;
				
				g_enable_state = false;
			}
			
			continue;
		}
		
		//************************
		//  current state is don't, so dont bother parsing multiply
		if(!g_enable_state)
		{
			i++;
			continue;
		}
		
		//************************
		//  find valid mul( prefix
		if(CheckChar(++i, 'u'))
			continue;
		if(CheckChar(++i, 'l'))
			continue;
		if(CheckChar(++i, '('))
			continue;
		
		i++;
		if(i >= count)
			return false;
		
		//************************
		//  find left hand input
		if(!AoC::CharIsNumber(g_str[i]))
			continue;
		
		int idx = 0;
		lhs = CharBuffToInt(g_str.GetSubSlice(i,GOL_MIN(4, (count-1) - i)), idx);
		if(lhs >= 1000) // number of digits > 3
			continue;
		
		i += idx;
		if(i >= count)
			return false;
		
		//************************
		//  verify valid comma
		if(CheckChar(i,','))
			continue;
		
		i++;
		if(i >= count)
			return false;
		
		//************************
		//  find right hand input
		idx = 0;
		rhs = CharBuffToInt(g_str.GetSubSlice(i,GOL_MIN(4, (count-1) - i)), idx);
		if(rhs >= 1000) // number of digits > 3
			continue;
		
		i += idx;
		if(i >= count)
			return false;
		
		//************************
		//  find valid parenthisis
		if(CheckChar(i,')'))
			continue;
		
		// yippee!! we found a valid mul(xxx,yyy) token!
		return true;
	}
	
	return false;
}
#endif

void day3(int _day)
{
	gol::File file = AoC::LoadData(_day);
	g_str = file.ReadText();
	file.Close();
	
	int total_sum = 0;
	{
		gol::Timer timer;
		
		int count = g_str.GetSize();
		for(int i = 0; i < count; i++)
		{
			int lhs, rhs;
			if(!FindValidMulToken(i, lhs, rhs))
				break;
		
			total_sum += lhs * rhs;
		}
	}
	
	printf("total_sum = %i\n", total_sum);
}