#include "../IntcodeComputer/IntcodeComputer.h"
#include <fstream>
#include <iostream>
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

uint64_t play(std::vector<int64_t> &program)
{
	uint64_t result = 0;
	
	std::array<std::string,26> screen;
	std::string line;
	for(unsigned i=0; i<37; i++)
	{
		line += ' ';
	}
	for(unsigned i=0; i<26; i++)
	{
		screen[i] = line;
	}
	
	IntcodeComputer comp;
	comp.loadProgram(program);
	comp.executeProgram();
	size_t x = 0;
	size_t y = 0;
	unsigned typeId = 0;
	
	while(!comp.outbox.empty())
	{
		x = comp.fetchValue();
		y = comp.fetchValue();
		typeId = comp.fetchValue();
		
		switch(typeId)
		{
			case 0:
			{
				screen[y][x] = ' ';
				break;
			}
			case 1:
			{
				screen[y][x] = '|';
				break;
			}
			case 2:
			{
				screen[y][x] = '#';
				break;
			}
			case 3:
			{
				screen[y][x] = '_';
				break;
			}
			case 4:
			{
				screen[y][x] = 'O';
			}
		}
	}
	
	for(auto &line:screen)
	{
		for(auto element:line)
		{
			if(element == '#')
			{
				result++;
			}
		}
	}
	
	return result;
}

uint64_t playForFree(std::vector<int64_t> &program)
{
	uint64_t result = 0;
	
	program[0] = 2;
	unsigned score = 0;
	
	std::array<std::string,26> screen;
	std::string line;
	for(unsigned i=0; i<37; i++)
	{
		line += ' ';
	}
	for(unsigned i=0; i<26; i++)
	{
		screen[i] = line;
	}
	
	IntcodeComputer comp;
	comp.loadProgram(program);
	int x = 0;
	size_t y = 0;
	unsigned typeId = 0;
	int control = 0;
	int prevX = 0;
	int paddle = 0;

	do
	{
		comp.executeProgram();
		
		while(!comp.outbox.empty())
		{
			prevX = x;
			x = comp.fetchValue();
			y = comp.fetchValue();
			typeId = comp.fetchValue();
			
			if(x == -1 && y == 0)
			{
				score = typeId;
			}
			else
			{
				switch(typeId)
				{
					case 0:
					{
						screen[y][x] = ' ';
						break;
					}
					case 1:
					{
						screen[y][x] = '|';
						break;
					}
					case 2:
					{
						screen[y][x] = '#';
						break;
					}
					case 3:
					{
						screen[y][x] = '-';
						paddle = x;
						break;
					}
					case 4:
					{
						screen[y][x] = 'O';
					}
				}
			}
		}
		std::cout << score << '\n';
		for(auto &line:screen)
		{
			std::cout << line << '\n';
		}
		std::cout << std::endl;
		
		control = 0;
		if(prevX < x)
		{
			if(paddle < x)
			{
				control = 1;
			}
			else if(paddle > x)
			{
				control = -1;
			}
		}
		else
		{
			if(paddle > x)
			{
				control = -1;
			}
			else if(paddle < x)
			{
				control = 1;
			}
		}
		comp.pushValue(control);
	} while(comp.isSleeping());
	
	result = score;
	
	return result;
}


int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<int64_t> program;
  parseInput(program);

	resultA = play(program);
	resultB = playForFree(program);

	std::cout << "ResultA: " << resultA << '\n';
	std::cout << "ResultB: " << resultB << '\n';
	
  return 0;
}
