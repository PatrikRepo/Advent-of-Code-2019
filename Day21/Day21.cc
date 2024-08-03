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

uint64_t walk(std::vector<int64_t> &program)
{
	uint64_t result = 0;
	
	IntcodeComputer comp;
	comp.loadProgram(program);
	
	std::array<std::string,7> inputs;
	
	inputs[0] = "NOT A J";
	inputs[1] = "NOT B T";
	inputs[2] = "OR T J";
	inputs[3] = "NOT C T";
	inputs[4] = "OR T J";
	inputs[5] = "AND D J";
	inputs[6] = "WALK";
	
	for(auto& input:inputs)
	{
		for(auto element:input)
		{
			comp.pushValue(element);
		}
		comp.pushValue(10);
	}
	
	comp.executeProgram();
	
	while(!comp.outbox.empty())
	{
		result = comp.fetchValue();
	}
	
	return result;
}

uint64_t run(std::vector<int64_t> &program)
{
	uint64_t result = 0;
	
	IntcodeComputer comp;
	comp.loadProgram(program);
	
	std::array<std::string,11> inputs;
	
	inputs[0] = "NOT A J";
	inputs[1] = "NOT B T";
	inputs[2] = "OR T J";
	inputs[3] = "NOT C T";
	inputs[4] = "OR T J";
	inputs[5] = "AND D J";
	inputs[6] = "NOT J T";
	inputs[7] = "OR E T";
	inputs[8] = "OR H T";
	inputs[9] = "AND T J";
	inputs[10] = "RUN";
	
	for(auto& input:inputs)
	{
		for(auto element:input)
		{
			comp.pushValue(element);
		}
		comp.pushValue(10);
	}
	
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
	
  parseInput(program);
	
	resultA = walk(program);
	
	resultB = run(program);

	std::cout << "ResultA: " << resultA << '\n';
	std::cout << "ResultB: " << resultB << '\n';
	
  return 0;
}
