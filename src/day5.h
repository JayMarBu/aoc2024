#include "AocUtils.h"

#include "gol/HashTable.h"
#include "gol/Maths.h"
#include "gol/Vec.h"
#include "gol/Vector.h"
#include "timer.h"
#include <stdio.h>

#define PART 2
#define DO_LOGGING 0

enum eDataType
{
	Type_UpdateHeader = 0,
	Type_Page = 1,
};

struct Update
{
	eDataType type;
	union
	{
		int count;
		int page_number;
	};
};

gol::Vector<Update> g_updates;
gol::HashTable<int, gol::Vector<int>> g_predecesor_map;

void GetData(gol::File file)
{
	bool read_all_ordering_rules = false;
	const size_t buff_len = 128;
	char buff[buff_len];
	while(file.ReadTextLine(buff, buff_len))
	{
		size_t line_count = gol::StrLen(buff);
		if(!read_all_ordering_rules)
		{
			if(AoC::CharIsLineEnd(buff[0]))
			{
				read_all_ordering_rules = true;
				continue;
			}
			
			int i = 0;
			int lhs = AoC::CharBuffToInt(buff, i);
			i++;
			int rhs = AoC::CharBuffToInt(buff, i);
			
			if(g_predecesor_map.Contains(rhs))
				g_predecesor_map[rhs].Push(lhs);
			else
			{
				g_predecesor_map.Set(rhs);
				g_predecesor_map[rhs].Push(lhs);
			}
		}
		else
		{
			int header_idx = g_updates.GetCount();
			Update& header = g_updates.Emplace();
			header.type = Type_UpdateHeader;
			header.count = 0;
			
			int i = 0;
			while(i < line_count && !AoC::CharIsLineEnd(buff[i]))
			{
				Update& page = g_updates.Emplace();
				page.type = Type_Page;
				page.page_number = AoC::CharBuffToInt(buff, i);
				i++;
				g_updates[header_idx].count++;
			}
		}
	}
}

bool IsInOrder(gol::Slice<Update> pages)
{
	for(int i = 0; i < pages.GetCount(); i++)
	{
		int curr_page = pages[i].page_number;
		if(!g_predecesor_map.Contains(curr_page))
			continue;
		
		gol::Slice<int> predecesors = g_predecesor_map[curr_page];
		
		for(int j = i+1; j < pages.GetCount(); j++)
		{
			for(int predecesor : predecesors)
			{
				if(predecesor == pages[j].page_number)
					return false;
			}
		}
	}
	
	return true;
}

bool ReOrder(gol::Slice<Update> pages)
{
	bool result = true;
	for(int i = 0; i < pages.GetCount(); i++)
	{
restart_point:
		int curr_page = pages[i].page_number;
		if(!g_predecesor_map.Contains(curr_page))
			continue;
		
		gol::Slice<int> predecesors = g_predecesor_map[curr_page];
		
		for(int j = i+1; j < pages.GetCount(); j++)
		{
			for(int predecesor : predecesors)
			{
				if(predecesor == pages[j].page_number)
				{
					result = false;
					gol::Swap(pages[j].page_number, pages[i].page_number);
					goto restart_point;
				}	
			}
		}
	}
	
	return result;
}

void day5(int _day)
{
	GetData(AoC::LoadData(_day));
	
	gol::Timer timer;
	
#if DO_LOGGING
	for(gol::HashTable<int, gol::Vector<int>>::Slot& slot : g_predecesor_map)
	{
		printf("%i | ", slot.key);
		
		for(int v : slot.val)
		{
			printf("%i,", v);
		}
		printf("\n");
	}
	printf("\n");
#endif
	
	int correct_total = 0;
	int incorrect_total = 0;
	for(int i = 0; i < g_updates.GetCount();)
	{
		Update& header = g_updates[i];
		int mid_idx = (int)gol::Ceil(header.count * 0.5f);
		int mid_pos = i + mid_idx;
		
#if PART == 1
		bool is_correct = IsInOrder(gol::Slice(&g_updates[i+1], header.count));
		if(is_correct)
			correct_total += g_updates[mid_pos].page_number;
#elif PART == 2
		bool is_correct = ReOrder(gol::Slice(&g_updates[i+1], header.count));
		if(is_correct)
			correct_total += g_updates[mid_pos].page_number;
		else
			incorrect_total += g_updates[mid_pos].page_number;
#endif
		
#if DO_LOGGING
		if(i < 10)
			printf("\nupdate[   %i] {%s} : ", i, is_correct ? "CORRECT  " : "INCORRECT");
		else if (i < 100)
			printf("\nupdate[  %i] {%s} : ", i, is_correct ? "CORRECT  " : "INCORRECT");
		else if (i < 1000)
			printf("\nupdate[ %i] {%s} : ", i, is_correct ? "CORRECT  " : "INCORRECT");
		else
			printf("\nupdate[%i] {%s} : ", i, is_correct ? "CORRECT  " : "INCORRECT");
		
		i++;
		
		int header_end = i + header.count;
		for(; i < header_end; i++)
		{
			if(i == mid_pos)
				printf("<%i> ", g_updates[i].page_number);
			else
				printf("%i ", g_updates[i].page_number);
		}
#else
		i = i + header.count+1;
#endif
	}
	
	printf("\ncorrect total   = %i {6949}\n", correct_total);
	printf("incorrect total = %i {4145}\n", incorrect_total);
}