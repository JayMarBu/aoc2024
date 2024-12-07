#include "AocUtils.h"

#include "gol/Vec.h"
#include "gol/Vector.h"
#include "gol/StringAlgorithms.h"
#include "timer.h"
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>


#define PART 1

enum eDirection
{
	NORTH 		= 0,
	EAST		= 1,
	SOUTH		= 2,
	WEST		= 3,
	
	MASK_NORTH 		= GOL_BIT(0),
	MASK_EAST		= GOL_BIT(1),
	MASK_SOUTH		= GOL_BIT(2),
	MASK_WEST		= GOL_BIT(3),
	
	MASK_ALL = MASK_NORTH | MASK_EAST | MASK_SOUTH | MASK_WEST
};

gol::Vec2i directions[4] =
{	
	{ 0, -1},// NORTH 		
	{ 1,  0},// EAST		
	{ 0,  1},// SOUTH		
	{-1,  0},// WEST
};

enum eTile : char
{
	EMPTY = '.',
	OBSTACLE = '#',
	//VISITED = 'o',
	GUARD_NORTH = '^',
	GUARD_SOUTH = 'V',
	GUARD_EAST = '>',
	GUARD_WEST = '<',
};

char directions_guard[4] =
{	
	GUARD_NORTH,
	GUARD_EAST,
	GUARD_SOUTH,
	GUARD_WEST,
};

struct Grid
{
	int width;
	int height;
	gol::Vector<char> data;
	
	bool OutOfBounds(gol::Vec2i xy)
	{
		return (xy.x < 0) || (xy.x >= width) || (xy.y < 0) || (xy.y >= height);
	}

	int GetIndex(int x, int y) 
	{
		gol_assert(x >= 0 && x < width);
		gol_assert(y >= 0 && y < height);
		
		return (width * y) + x;
	}
	
	char& operator[](gol::Vec2i xy) { return data[GetIndex(xy.x,xy.y)]; } 
	
	void Draw()
	{
		COORD pos = {0, 0};
		HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(output, pos);

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < height; x++)
			{
				printf("%c", data[GetIndex(x, y)]);
			}
			printf("\n");
		}
	}

	void DrawTile(gol::Vec2i xy)
	{
		HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_SCREEN_BUFFER_INFO ConsoleScreenBufferInfo;
		if (GetConsoleScreenBufferInfo(output, &ConsoleScreenBufferInfo))
		{

		}

		COORD pos = {(SHORT)xy.x, (SHORT)xy.y};
		
		bool result = SetConsoleCursorPosition(output, pos);
		if (!result)
		{
			DWORD error = GetLastError();;
		}
		printf("%c", data[GetIndex(xy.x, xy.y)]);
	}

	void Init(gol::File file, gol::Vec2i& guardStartPos)
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
		height = 0;
	
		data.Reserve(line_size * line_size);
	
		do
		{
			s32 idx = gol::FindFirstCharInStr(buff, "^v><");
			if (idx != -1)
				guardStartPos = { idx, height };
			data.Push(gol::Slice<char>(buff, line_size));
			height++;
		}
		while(file.ReadTextLine(buff, buff_len));
		
		file.Close();
	}
};

bool IsVisited(char tile)
{
	return tile == GUARD_NORTH || tile == GUARD_SOUTH || tile == GUARD_EAST || tile == GUARD_WEST;
}

void day6(int _day)
{
	gol::Vec2i guard_pos;
	s32 guard_direction;

	Grid grid;
	grid.Init(AoC::LoadData(_day), guard_pos);

	switch (grid[guard_pos])
	{
	case GUARD_NORTH: guard_direction = NORTH; break;
	case GUARD_SOUTH: guard_direction = SOUTH; break;
	case GUARD_EAST:  guard_direction = EAST;  break;
	case GUARD_WEST:  guard_direction = WEST;  break;
	}
	//grid[guard_pos] = VISITED;

	grid.Draw();

	s32 step_count = 1;
	s32 loop_count = 0;
	bool out_of_bounds = grid.OutOfBounds(guard_pos + directions[guard_direction]);
	while (!out_of_bounds)
	{
		gol::Vec2i pos = guard_pos + directions[guard_direction];	
		char tile = grid[pos];
		if (tile == OBSTACLE)
		{
			guard_direction = (guard_direction + 1) % 4;
			grid[guard_pos] = directions_guard[guard_direction];
			grid.DrawTile(guard_pos);
			continue;
		}

		bool visited = IsVisited(tile);

		if (!visited)
			step_count++;


		if (!grid.OutOfBounds(pos + directions[guard_direction]))
		{
			s32 loop_direction = (guard_direction + 1) % 4;
			gol::Vec2i loop = pos + directions[loop_direction];
			if (visited)
			{
				if (tile == directions_guard[loop_direction])
					loop_count++;
			}
			else
			{
				while (!grid.OutOfBounds(loop))
				{
					if (grid[loop] == OBSTACLE)
						break;

					if (grid[loop] == directions_guard[loop_direction])
					{
						loop_count++;
						break;
					}
					loop += directions[loop_direction];
				}

				
			}
		} 

		guard_pos = pos;
		grid[pos] = directions_guard[guard_direction];

		grid.DrawTile(guard_pos);
		out_of_bounds = grid.OutOfBounds(guard_pos + directions[guard_direction]);
	}

	COORD pos = {0, grid.height+1};
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);

	printf("step_count = %i\n", step_count);
	printf("loop_count = %i\n", loop_count);
}