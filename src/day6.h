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

		///CONSOLE_SCREEN_BUFFER_INFO ConsoleScreenBufferInfo;
		//if (GetConsoleScreenBufferInfo(output, &ConsoleScreenBufferInfo))
		//{

		//}

		COORD pos = {(SHORT)xy.x, (SHORT)xy.y};
		
		bool result = SetConsoleCursorPosition(output, pos);
		//if (!result)
		//{
		//	DWORD error = GetLastError();;
		//}
		printf("%c", data[GetIndex(xy.x, xy.y)]);
	}
	
	void DrawTempTile(gol::Vec2i xy, char c)
	{
		HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

		//CONSOLE_SCREEN_BUFFER_INFO ConsoleScreenBufferInfo;
		//if (GetConsoleScreenBufferInfo(output, &ConsoleScreenBufferInfo))
		//{

		//}

		COORD pos = {(SHORT)xy.x, (SHORT)xy.y};
		
		bool result = SetConsoleCursorPosition(output, pos);
		//if (!result)
		//{
		//	DWORD error = GetLastError();;
		//}
		printf("%c", c);
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

bool do_intense_draw = false;

struct Node
{
	s64 next = 0;
	s64 prev = 0;
	s32 distance_next = 0;
	s32 distance_prev = 0;
		
	static s64 ComputeID(gol::Vec2i pos, gol::Vec2i bounds, s32 dir)
	{
		return 4 * (pos.x * bounds.width + pos.y) + dir;
	}
};

void AddTreeNode(gol::HashTable<s64, Node>& tree, Grid& grid, gol::Vec2i pos, s32 dir)
{
	gol::Vec2i pos += directions[dir + 4 % 4];
	s64 id = Node::ComputeID(pos, {grid.width, grid.height}, SOUTH);
	if(!tree.Contains(id))
		tree.Set(id);
				
	Node& node = tree[id];
	node.distance_next = 1;
				
	while(!grid.OutOfBounds(pos + directions[SOUTH]) && grid[pos + directions[SOUTH]] != OBSTACLE)
	{
		node.distance_next++;
		pos += directions[SOUTH];
	}
				
	pos -= directions[SOUTH];
	s64 other_id = Node::ComputeID(pos, {grid.width, grid.height}, WEST);
	if(!tree.Contains(other_id))
		tree.Set(other_id);
				
	Node& other_node = tree[other_id];
	other_node.prev = id;
	other_node.distance_prev = node.distance_next;
				
	node.next = other_id;
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
	
#if PART == 1
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
		
		guard_pos = pos;
		grid[pos] = directions_guard[guard_direction];
		grid.DrawTile(guard_pos);
		
		out_of_bounds = grid.OutOfBounds(guard_pos + directions[guard_direction]);
	}
	
	COORD pos = {0, grid.height+1};
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
#endif
	
	gol::HashTable<s64, Node> tree;
	
	for(int y = 0; y < grid.height; y++)
	{
		for(int x = 0; x < grid.width; x++)
		{
			if(grid[{x,y}] != OBSTACLE)
				continue;
			
			if(x>0 && grid[{x-1, y}] != OBSTACLE)
			{
				gol::Vec2i pos = {x-1, y};
				s64 id = Node::ComputeID(pos, {grid.width, grid.height}, SOUTH);
				if(!tree.Contains(id))
					tree.Set(id);
				
				Node& node = tree[id];
				node.distance_next = 1;
				
				while(!grid.OutOfBounds(pos + directions[SOUTH]) && grid[pos + directions[SOUTH]] != OBSTACLE)
				{
					node.distance_next++;
					pos += directions[SOUTH];
				}
				
				pos -= directions[SOUTH];
				s64 other_id = Node::ComputeID(pos, {grid.width, grid.height}, WEST);
				if(!tree.Contains(other_id))
					tree.Set(other_id);
				
				Node& other_node = tree[other_id];
				other_node.prev = id;
				other_node.distance_prev = node.distance_next;
				
				node.next = other_id;
			}
		}	
	}
	
	step_count = 1;
	
	
	

	printf("step_count = %i\n", step_count);
	printf("loop_count = %i\n", loop_count);
}