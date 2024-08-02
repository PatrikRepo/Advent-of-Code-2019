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

uint64_t scanBeam(std::array<std::string, 50> &map, std::vector<int64_t> &baseProgram)
{
	uint64_t result = 0;	
	
	std::string line;
	for(unsigned i=0; i<50; i++)
	{
		line += ' ';
	}
	
	for(unsigned i=0; i<50; i++)
	{
		map[i] = line;
	}
	
	unsigned output = 0;
	
	for(unsigned i=0; i<50; i++)
	{
		for(unsigned j=0; j<50; j++)
		{
			auto program = baseProgram;
			IntcodeComputer comp;
			comp.loadProgram(program);
			comp.pushValue(j);
			comp.pushValue(i);
			comp.executeProgram();
			output = comp.fetchValue();
			
			if(output == 0)
			{
				map[i][j] = '.';
			}
			else
			{
				map[i][j] = '#';
			}
		}
	}
	
	for(auto& line:map)
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

uint64_t shipCoordinates(std::vector<int64_t> &baseProgram)
{
	uint64_t result = 0;	
	
	unsigned output = 0;
	bool done = false;
	size_t x = 381;
	size_t y = 624;
	
	while(!done)
	{
		auto program = baseProgram;
		IntcodeComputer comp;
		comp.loadProgram(program);
		comp.pushValue(x);
		comp.pushValue(y);
		comp.executeProgram();
		
		output = comp.fetchValue();
		if(output == 0)
		{
			x++;
		}
		else
		{
			auto copy1 = baseProgram;
			auto copy2 = baseProgram;
			IntcodeComputer compCopy1;
			IntcodeComputer compCopy2;
			compCopy1.loadProgram(copy1);
			compCopy2.loadProgram(copy2);
			compCopy1.pushValue(x);
			compCopy1.pushValue(y-99);
			compCopy2.pushValue(x+99);
			compCopy2.pushValue(y-99);
			compCopy1.executeProgram();
			compCopy2.executeProgram();
			unsigned output1 = compCopy1.fetchValue();
			unsigned output2 = compCopy2.fetchValue();
			
			if(output1 == 1 && output2 == 1)
			{
				result = (10000*x) + y-99;
				done = true;
				break;
			}
			else
			{
				y++;
			}
		}
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<int64_t> program;
	std::array<std::string, 50> map;
	
  parseInput(program);
	
	resultA = scanBeam(map, program);
	
	resultB = shipCoordinates(program);

	std::cout << "ResultA: " << resultA << '\n';
	std::cout << "ResultB: " << resultB << '\n';
	
  return 0;
}
