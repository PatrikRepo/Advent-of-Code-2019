#include "../IntcodeComputer/IntcodeComputer.h"
#include <fstream>
#include <array>
#include <iostream>
#include <algorithm>

void parseInput(std::vector<int> &program)
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
			program.push_back(std::stoi(line.substr(pos,endpos-pos)));
  		pos = line.find_first_of("-1234567890",endpos);
		}
  }
  input.close();
}

uint64_t calculateSignal(std::vector<int> &program)
{
	uint64_t result = 0;
	
	std::array<unsigned,5> phases{0,1,2,3,4};
	do
	{
		uint64_t tempResult = 0;
		for(unsigned i=0; i<5; i++)
		{
			IntcodeComputer comp;
			comp.loadProgram(program);
			comp.inbox.push(phases[i]);
			comp.inbox.push(tempResult);
			comp.executeProgram();
			tempResult = comp.outbox.front();
			comp.outbox.pop();
		}
		if(result < tempResult)
		{
			result = tempResult;
		}
	}
	while(std::next_permutation(phases.begin(),phases.end()));

	return result;
}

uint64_t feedbackLoop(std::vector<int> &program)
{
	uint64_t result = 0;
	
	std::array<unsigned,5> phases{5,6,7,8,9};
	do
	{
		IntcodeComputer compA;
		compA.loadProgram(program);
		compA.inbox.push(phases[0]);
		
		IntcodeComputer compB;
		compB.loadProgram(program);
		compB.inbox.push(phases[1]);
		
		IntcodeComputer compC;
		compC.loadProgram(program);
		compC.inbox.push(phases[2]);
		
		IntcodeComputer compD;
		compD.loadProgram(program);
		compD.inbox.push(phases[3]);
		
		IntcodeComputer compE;
		compE.loadProgram(program);
		compE.inbox.push(phases[4]);
		
		uint64_t tempResult = 0;

		do
		{
			compA.inbox.push(tempResult);
			compA.executeProgram();
			tempResult = compA.outbox.front();
			compA.outbox.pop();

			compB.inbox.push(tempResult);
			compB.executeProgram();
			tempResult = compB.outbox.front();
			compB.outbox.pop();

			compC.inbox.push(tempResult);
			compC.executeProgram();
			tempResult = compC.outbox.front();
			compC.outbox.pop();

			compD.inbox.push(tempResult);
			compD.executeProgram();
			tempResult = compD.outbox.front();
			compD.outbox.pop();
			
			compE.inbox.push(tempResult);
			compE.executeProgram();
			tempResult = compE.outbox.front();
			compE.outbox.pop();
		}
		while(compE.sleeping);

		if(result < tempResult)
		{
			result = tempResult;
		}
	}
	while(std::next_permutation(phases.begin(),phases.end()));

	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<int> program;
  parseInput(program);

	resultA = calculateSignal(program);
	resultB = feedbackLoop(program);

	std::cout << "ResultA: " << resultA << '\n';
	std::cout << "ResultB: " << resultB << std::endl;

  return 0;
}
