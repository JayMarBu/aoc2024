#include "AocUtils.h"

#include "gol/Vec.h"
#include "gol/Vector.h"
#include "timer.h"
#include <stdio.h>

#define PART 2

struct Grid
{
	int width;
	int height;
	gol::Vector<char> data;
	
	int GetIndex(int x, int y) 
	{
		gol_assert(x >= 0 && x < width);
		gol_assert(y >= 0 && y < height);
		
		return (width * y) + x;
	}
	
	char& operator[](gol::Vec2i xy) { return data[GetIndex(xy.x,xy.y)]; } 
	
	void Init(gol::File file)
	{
		constexpr int buff_len = 512;
		char buff[buff_len];
		file.ReadTextLine(buff, buff_len);
		size_t line_size = gol::StrLen(buff);
	
		// account for end line characters
		for(int i = line_size; i >= 0; i--)
		{
			if(buff[i] != 0 && !AoC::CharIsLineEnd(buff[i]))
			{
				line_size = i+1;
				break;
			}
		
			buff[i] = 0;
		}
	
		width = line_size;
		height = line_size;
	
		data.Reserve(width * height);
	
		do
		{
			data.Push(gol::Slice<char>(buff, line_size));
		}
		while(file.ReadTextLine(buff, buff_len));
		
		file.Close();
	}
};

enum eDirection
{
	NORTH 		= 0,
	EAST		= 1,
	SOUTH		= 2,
	WEST		= 3,
	
	NORTH_EAST	= 4,
	SOUTH_EAST	= 5,
	SOUTH_WEST	= 6,
	NORTH_WEST	= 7,
	
	MASK_NORTH 		= GOL_BIT(0),
	MASK_EAST		= GOL_BIT(1),
	MASK_SOUTH		= GOL_BIT(2),
	MASK_WEST		= GOL_BIT(3),
	
	MASK_NORTH_EAST	= MASK_NORTH | MASK_EAST,
	MASK_SOUTH_EAST	= MASK_SOUTH | MASK_EAST,
	MASK_NORTH_WEST	= MASK_NORTH | MASK_WEST,
	MASK_SOUTH_WEST	= MASK_SOUTH | MASK_WEST,
	
	MASK_ALL = MASK_NORTH | MASK_EAST | MASK_SOUTH | MASK_WEST
};

gol::Vec2i direction_idx[8] =
{	
	{ 0, -1},// NORTH 		
	{ 1,  0},// EAST		
	{ 0,  1},// SOUTH		
	{-1,  0},// WEST
	
	{ 1, -1},// NORTH_EAST
	{ 1,  1},// SOUTH_EAST
	{-1,  1},// SOUTH_WEST
	{-1, -1},// NORTH_WEST
};

const char* direction_str[8] =
{	
	"NORTH",
	"EAST",
	"SOUTH",
	"WEST",
	
	"NORTH_EAST",
	"SOUTH_EAST",
	"SOUTH_WEST",
	"NORTH_WEST",
};

#if PART == 1
const char* word = "XMAS";
const size_t word_len = gol::StrLen(word);

bool CheckDirection(Grid& grid, gol::Vec2i origin, gol::Vec2i direction)
{
	gol::Vec2i idx = origin + direction;
	for(int i = 1; i < word_len; i++)
	{
		char c = grid[idx];
		if(c != word[i])
			return false;
		
		idx += direction;
	}
	return true;
}

int Run(Grid& grid)
{
	gol::Timer timer;
	
	int word_count = 0;
	for(int y = 0; y < grid.height; y++)
	{
		const bool bcan_check_north = (y >= word_len-1);
		const bool bcan_check_south = (y <= grid.height - word_len);
		
		const u8 can_check_north = (MASK_NORTH & (bcan_check_north << NORTH));
		const u8 can_check_south = (MASK_SOUTH & (bcan_check_south << SOUTH));
		const u8 dirs_to_check_vertical = can_check_north | can_check_south;
		
		for(int x = 0; x < grid.width; x++)
		{
			if(grid[{x,y}] != word[0])
				continue;
			
			const bool bcan_check_west = (x >= word_len-1);
			const bool bcan_check_east = (x <= grid.width - word_len);
			
			const u8 can_check_east = (MASK_EAST & (bcan_check_east << EAST));
			const u8 can_check_west = (MASK_WEST & (bcan_check_west << WEST));
			const u8 dirs_to_check = dirs_to_check_vertical | can_check_east | can_check_west;
			
			if((dirs_to_check & MASK_NORTH) == MASK_NORTH)
			{
				if(CheckDirection(grid, {x,y}, direction_idx[NORTH]))
					word_count++;
			}
				
			if((dirs_to_check & MASK_NORTH_EAST) == MASK_NORTH_EAST)
			{
				if(CheckDirection(grid, {x,y}, direction_idx[NORTH_EAST]))
					word_count++;
			}
				
			if((dirs_to_check & MASK_EAST) == MASK_EAST)
			{
				if(CheckDirection(grid, {x,y}, direction_idx[EAST]))
					word_count++;
			}
				
			if((dirs_to_check & MASK_SOUTH_EAST) == MASK_SOUTH_EAST)
			{
				if(CheckDirection(grid, {x,y}, direction_idx[SOUTH_EAST]))
					word_count++;
			}
				
			if((dirs_to_check & MASK_SOUTH) == MASK_SOUTH)
			{
				if(CheckDirection(grid, {x,y}, direction_idx[SOUTH]))
					word_count++;
			}
				
			if((dirs_to_check & MASK_SOUTH_WEST) == MASK_SOUTH_WEST)
			{
				if(CheckDirection(grid, {x,y}, direction_idx[SOUTH_WEST]))
					word_count++;
			}
				
			if((dirs_to_check & MASK_WEST) == MASK_WEST)
			{
				if(CheckDirection(grid, {x,y}, direction_idx[WEST]))
					word_count++;
			}
			
			if((dirs_to_check & MASK_NORTH_WEST) == MASK_NORTH_WEST)
			{
				if(CheckDirection(grid, {x,y}, direction_idx[NORTH_WEST]))
					word_count++;
			}
		}
	}
	return word_count;
}

#elif PART == 2
const char* word = "MAS";
const size_t word_len = gol::StrLen(word);

bool CheckDirection(Grid& grid, gol::Vec2i origin, gol::Vec2i direction)
{
	gol::Vec2i idx = origin + direction;
	gol::Vec2i inv_idx = origin - direction;
	
	if(grid[idx] == word[0] && grid[inv_idx] == word[2])
		return true;
	
	if(grid[idx] == word[2] && grid[inv_idx] == word[0])
		return true;
	
	return false;
}

int Run(Grid& grid)
{
	gol::Timer timer;
	
	int word_count = 0;
	for(int y = 1; y < grid.height-1; y++)
	{
		for(int x = 1; x < grid.width-1; x++)
		{
			if(grid[{x,y}] != word[1])
				continue;
			
			bool found_back_slash = CheckDirection(grid, {x,y}, direction_idx[NORTH_EAST]);
			bool found_front_slash = CheckDirection(grid, {x,y}, direction_idx[NORTH_WEST]);
			
			if(found_back_slash && found_front_slash)
				word_count++;
		}
	}
	return word_count;
}
#endif

void day4(int _day)
{
	Grid grid;
	grid.Init(AoC::LoadData(_day));
	
	int word_count = Run(grid);
	
	printf("word count: %i\n", word_count);
}