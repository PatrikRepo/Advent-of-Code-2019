#include "../IntcodeComputer/IntcodeComputer.h"
#include <fstream>
#include <iostream>
#include <array>
#include <deque>

struct Node
{
	size_t x = 0;
	size_t y = 0;
	size_t steps = 0;
};

enum Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

void parseInput(std::vector<int64_t> &program)
{
  std::string line;
  size_t pos = 0;
  size_t endpos = 0;
  
  std::ifstream input("input.txt");

  if(input.is_open())
  {	
  	getline(input,line);
  	pos = line.find_first_of("-1234567890");
		while(pos != std::string::npos)
		{
			endpos = line.find_first_of(",",pos);
			program.push_back(std::stoll(line.substr(pos,endpos-pos)));
  		pos = line.find_first_of("-1234567890",endpos);
		}
  }
  input.close();
}

int translateDirection(int direction)
{
	int result = 0;
	switch(direction)
	{
		case Direction::UP :
		{
			result = 1;
			break;
		}
		case Direction::RIGHT :
		{
			result = 4;
			break;
		}
		case Direction::DOWN :
		{
			result = 2;
			break;
		}
		case Direction::LEFT :
		{
			result = 3;
		}
	}
	
	return result;
}

void drawMap(std::array<std::string, 41> &map, const std::vector<int64_t> &program)
{
	size_t x = 21;
	size_t y = 21;
	int direction = Direction::UP;
	
	size_t startX = x;
	size_t startY = y;
	int startDirection = direction;
	
	IntcodeComputer comp;
	comp.loadProgram(program);
	
	int output = 0;
	
	std::string line;
	for(unsigned i=0; i<41; i++)
	{
		line += '#';
	}
	for(unsigned i=0; i<41; i++)
	{
		map[i] = line;
	}
	
	do
	{
		int newDirection = (direction+3)%4;
		comp.pushValue(translateDirection(newDirection));
		comp.executeProgram();
		
		output = comp.fetchValue();
		
		if(output == 0)
		{
			direction = (direction+1)%4;
		}
		else
		{
			direction = newDirection;
			switch(direction)
			{
				case Direction::UP :
				{
					y--;
					break;
				}
				case Direction::RIGHT :
				{
					x++;
					break;
				}
				case Direction::DOWN :
				{
					y++;
					break;
				}
				case Direction::LEFT : 
				{
					x--;
					break;
				}
			}
			if(output == 1)
			{
				map[y][x] = '.';
			}
			else
			{
				map[y][x] = 'O';
			}
		}
	}
	while(startX != x || startY != y || direction != startDirection);
	
	map[startY][startX] = 'D';
}

uint64_t findOxygen(std::array<std::string, 41> map)
{
	uint64_t result = 0;
	
	size_t startX = 0;
	size_t startY = 0;
	
	for(unsigned i=0; i<map.size(); i++)
	{
		for(unsigned j=0; j<map[0].size(); j++)
		{
			if(map[i][j] == 'D')
			{
				startX = j;
				startY = i;
			}
		}
	}
	
	std::deque<Node> openList;
	
	Node node;
	node.x = startX;
	node.y = startY;
	openList.push_back(node);
	
	while(!openList.empty())
	{
		node = openList.front();
		openList.pop_front();
		
		if(map[node.y][node.x] == 'O')
		{
			result = node.steps;
			break;
		}
		map[node.y][node.x] = '#';
		
		Node newNode;
		
		if(map[node.y-1][node.x] != '#')
		{
			newNode.x = node.x;
			newNode.y = node.y-1;
			newNode.steps = node.steps+1;
			openList.push_back(newNode);
		}
		if(map[node.y][node.x+1] != '#')
		{
			newNode.x = node.x+1;
			newNode.y = node.y;
			newNode.steps = node.steps+1;
			openList.push_back(newNode);
		}
		if(map[node.y+1][node.x] != '#')
		{
			newNode.x = node.x;
			newNode.y = node.y+1;
			newNode.steps = node.steps+1;
			openList.push_back(newNode);
		}
		if(map[node.y][node.x-1] != '#')
		{
			newNode.x = node.x-1;
			newNode.y = node.y;
			newNode.steps = node.steps+1;
			openList.push_back(newNode);
		}
	}
	
	return result;
}

uint64_t fillOxygen(std::array<std::string, 41> &map)
{
	uint64_t result = 0;
	
	size_t startX = 0;
	size_t startY = 0;
	
	for(unsigned i=0; i<map.size(); i++)
	{
		for(unsigned j=0; j<map[0].size(); j++)
		{
			if(map[i][j] == 'O')
			{
				startX = j;
				startY = i;
			}
		}
	}
	
	std::deque<Node> openList;
	
	Node node;
	node.x = startX;
	node.y = startY;
	openList.push_back(node);
	
	while(!openList.empty())
	{
		node = openList.front();
		openList.pop_front();
		
		if(result < node.steps)
		{
			result = node.steps;
		}
		map[node.y][node.x] = '#';
		
		Node newNode;
		
		if(map[node.y-1][node.x] != '#')
		{
			newNode.x = node.x;
			newNode.y = node.y-1;
			newNode.steps = node.steps+1;
			openList.push_back(newNode);
		}
		if(map[node.y][node.x+1] != '#')
		{
			newNode.x = node.x+1;
			newNode.y = node.y;
			newNode.steps = node.steps+1;
			openList.push_back(newNode);
		}
		if(map[node.y+1][node.x] != '#')
		{
			newNode.x = node.x;
			newNode.y = node.y+1;
			newNode.steps = node.steps+1;
			openList.push_back(newNode);
		}
		if(map[node.y][node.x-1] != '#')
		{
			newNode.x = node.x-1;
			newNode.y = node.y;
			newNode.steps = node.steps+1;
			openList.push_back(newNode);
		}
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<int64_t> program;
	std::array<std::string, 41> map;
	
  parseInput(program);
	
	drawMap(map, program);
	resultA = findOxygen(map);
	
	resultB = fillOxygen(map);

	std::cout << "ResultA: " << resultA << '\n';
	std::cout << "ResultB: " << resultB << '\n';
	
  return 0;
}
