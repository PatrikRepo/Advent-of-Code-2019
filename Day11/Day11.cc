#include "../IntcodeComputer/IntcodeComputer.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <array>

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

void paintHull(const std::vector<int64_t> &program, std::vector<std::string> &hull, size_t x, size_t y)
{
	unsigned direction = Direction::UP;
	IntcodeComputer comp;
	comp.loadProgram(program);

	do
	{
		if(hull[y][x] == '#')
		{
			comp.pushValue(1);
		}
		else
		{
			comp.pushValue(0);
		}
		comp.executeProgram();
		
		unsigned colour = comp.fetchValue();
		unsigned turn = comp.fetchValue();
		
		if(colour == 0)
		{
			hull[y][x] = ' ';
		}
		else
		{
			hull[y][x] = '#';
		}
		if(turn == 1)
		{
			direction = (direction+1)%4;
		}
		else
		{
			direction = (direction+3)%4;
		}
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
			}	
		}
	}	while(comp.isSleeping());
}

uint64_t paintBlackHull(const std::vector<int64_t> &program)
{
	uint64_t result = 0;
	
	std::string line;
	std::vector<std::string> hull;
	
	for(unsigned i=0; i<101; i++)
	{
		line += '.';
	}
	for(unsigned i=0; i<68; i++)
	{
		hull.push_back(line);
	}
	
	size_t x = 70;
	size_t y = 30;
	
	paintHull(program, hull, x, y);
	
	for(auto &line:hull)
	{
		for(auto element:line)
		{
			if(element != '.')
			{
				result++;
			}
		}
	}
	return result;
}

uint64_t paintWhiteHull(const std::vector<int64_t> &program)
{
	uint64_t result = 0;
	
	std::string line;
	std::vector<std::string> hull;
	
	for(unsigned i=0; i<40; i++)
	{
		line += '.';
	}
	for(unsigned i=0; i<6; i++)
	{
		hull.push_back(line);
	}
	
	size_t x = 0;
	size_t y = 0;
	hull[y][x] = '#';
	
	paintHull(program, hull, x, y);
	
	for(auto &line:hull)
	{
		std::cout << line << std::endl;
	}
	return result;
}

int main()
{
	uint64_t resultA = 0;
	
	std::vector<int64_t> program;
  parseInput(program);

	resultA = paintBlackHull(program);

	std::cout << "ResultA: " << resultA << std::endl;
	
	paintWhiteHull(program);
	
  return 0;
}
