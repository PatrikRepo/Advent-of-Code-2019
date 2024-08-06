#include "../IntcodeComputer/IntcodeComputer.h"
#include <fstream>
#include <iostream>
#include <vector>

//Take:
//tambourine
//monolith
//coin
//mug


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

void findSanta(std::vector<int64_t> &program)
{
	IntcodeComputer comp;
	comp.loadProgram(program);
	std::string input;
	
	while(input != "QUIT")
	{
		comp.executeProgram();
		
		while(!comp.outbox.empty())
		{
			std::cout << (char)comp.fetchValue();
		}
		
		std::getline(std::cin,input);
		
		for(auto element:input)
		{
			comp.pushValue(element);
		}
		comp.pushValue(10);
	}
}

int main()
{
	uint64_t resultA = 0;
	int64_t resultB = 0;
	
	std::vector<int64_t> program;
	
  parseInput(program);
	
	findSanta(program);
	
/*	std::cout << "ResultA: " << resultA << '\n';
	std::cout << "ResultB: " << resultB << '\n';*/
	
  return 0;
}
