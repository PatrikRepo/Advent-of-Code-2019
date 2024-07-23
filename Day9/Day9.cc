#include "../IntcodeComputer/IntcodeComputer.h"
#include <fstream>
#include <array>
#include <iostream>
#include <algorithm>

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

void testBOOST(const std::vector<int64_t> &program)
{
	IntcodeComputer comp;
	
	comp.loadProgram(program);
	comp.inbox.push(1);
	comp.executeProgram();

	std::cout << "ResultA: " << '\n';
	while(!comp.outbox.empty())
	{
		std::cout << comp.outbox.front() << std::endl;
		comp.outbox.pop();
	}
}

void BOOST(const std::vector<int64_t> &program)
{
	IntcodeComputer comp;
	
	comp.loadProgram(program);
	comp.inbox.push(2);
	comp.executeProgram();

			std::cout << "ResultB: " << '\n';
	while(!comp.outbox.empty())
	{
		std::cout << comp.outbox.front() << std::endl;
		comp.outbox.pop();
	}
}

int main()
{
	std::vector<int64_t> program;
  parseInput(program);

	testBOOST(program);
	
	BOOST(program);

  return 0;
}
