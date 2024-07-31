#include "../IntcodeComputer/IntcodeComputer.h"
#include <fstream>
#include <iostream>
#include <array>

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

unsigned countNeighbors(const std::array<std::string, 40> &map, size_t i, size_t j)
{
	unsigned result = 0;
	
	if(map[i-1][j] == '#')
	{
		result++;
	}
	if(map[i][j+1] == '#')
	{
		result++;
	}
	if(map[i+1][j] == '#')
	{
		result++;
	}
	if(map[i][j-1] == '#')
	{
		result++;
	}
	
	return result;
}

uint64_t drawMap(std::array<std::string, 40> &map, const std::vector<int64_t> &program)
{
	uint64_t result = 0;	
	
	IntcodeComputer comp;
	comp.loadProgram(program);
	comp.executeProgram();
	
	int output = 0;
	
	std::string line;
	for(unsigned i=0; i<60; i++)
	{
		line += 'X';
	}
	for(unsigned i=0; i<40; i++)
	{
		map[i] = line;
	}
	
	unsigned y = 0;
	unsigned x = 0;

	while(!comp.outbox.empty())
	{
		output = comp.fetchValue();
		
		if(output == 10)
		{
			y++;
			x = 0;
		}
		else
		{
			map[y][x] = output;
			x++;
		}
	}
	
	for(unsigned i=0; i<map.size(); i++)
	{
		for(unsigned j=0; j<map[0].size(); j++)
		{
			if(map[i][j] == '#' && countNeighbors(map, i, j) == 4)
			{
				result += i*j;
			}
		}
	}
	
	return result;
}

uint64_t vacuum(const std::array<std::string, 40> &map, std::vector<int64_t> &program)
{
	uint64_t result = 0;	
	
	program[0] = 2;
	
	IntcodeComputer comp;
	comp.loadProgram(program);
	
	std::string main = "A,B,B,C,C,A,A,B,B,C";
	std::string A = "L,12,R,4,R,4";
	std::string B = "R,12,R,4,L,12";
	std::string C = "R,12,R,4,L,6,L,8,L,8";
	
	for(auto element:main)
	{
		comp.pushValue(element);
	}
	comp.pushValue(10);
	for(auto element:A)
	{
		comp.pushValue(element);
	}
	comp.pushValue(10);
	for(auto element:B)
	{
		comp.pushValue(element);
	}
	comp.pushValue(10);
	for(auto element:C)
	{
		comp.pushValue(element);
	}
	comp.pushValue(10);
	comp.pushValue('n');
	comp.pushValue(10);
	
	comp.executeProgram();

	while(!comp.outbox.empty())
	{
		result = comp.fetchValue();
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<int64_t> program;
	std::array<std::string, 40> map;
	
  parseInput(program);
	
	resultA = drawMap(map, program);
	
	resultB = vacuum(map, program);

	std::cout << "ResultA: " << resultA << '\n';
	std::cout << "ResultB: " << resultB << '\n';
	
  return 0;
}
