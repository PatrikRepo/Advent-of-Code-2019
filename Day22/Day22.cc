#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <bitset>
#include <boost/multiprecision/cpp_int.hpp>

struct Instruction
{
	char technique = 0;
	boost::multiprecision::cpp_int number = 0;
};

void parseInput(std::vector<Instruction> &instructions)
{
	std::string line;
	
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
  	while(getline(input, line))
  	{
  		size_t pos = 0;
  		Instruction instruction;
  		if(line[0] == 'c')
  		{
  			pos = line.find_first_of("-1234567890");
  			if(line[pos] == '-')
  			{
  				instruction.technique = 'n';
  				pos++;
  			}
  			else
  			{
  				instruction.technique = 'c';
  			}
  			instruction.number = std::stoi(line.substr(pos));
  		}
  		else
  		{
  			pos = line.find_first_of("1234567890");
  			if(pos == std::string::npos)
  			{
  				instruction.technique = 'r';
  			}
  			else
  			{
  				instruction.technique = 'd';
  				instruction.number = std::stoi(line.substr(pos));
  			}
  		}
  		instructions.push_back(instruction);
  	}
  }
  input.close();
}

boost::multiprecision::cpp_int inverseMod(boost::multiprecision::cpp_int a, boost::multiprecision::cpp_int b)
{
	boost::multiprecision::cpp_int result = 0;
	
	boost::multiprecision::cpp_int x = 1;
	boost::multiprecision::cpp_int y = 0;
	boost::multiprecision::cpp_int mod = b;
	
  boost::multiprecision::cpp_int lastX = 0;
  boost::multiprecision::cpp_int lastY = 1;
  boost::multiprecision::cpp_int q = 0;
  boost::multiprecision::cpp_int r = 0;
  boost::multiprecision::cpp_int m = 0;
  boost::multiprecision::cpp_int n = 0;
  while (a != 0) 
  {
    q = b/a;
    r = b%a;
    m = lastX - (q*x);
    n = lastY - (q*y);
    lastX = x; 
    lastY = y;
    x = m; 
    y = n;
    b = a; 
    a = r;
  }
	

	if(lastX < 0)
	{
		lastX += mod;
	}
	result = lastX;
	return result;
}

void collapseInstructions(std::vector<Instruction> &instructions, boost::multiprecision::cpp_int deckSize)
{
	for(unsigned i=0; i<instructions.size(); i++)
	{
		if(instructions[i].technique == 'r')
		{
			instructions[i].technique = 'd';
			instructions[i].number = deckSize-1;
			Instruction instruction;
			instruction.technique = 'c';
			instruction.number = 1;
			instructions.insert(instructions.begin()+i+1,instruction);
		}
		if(instructions[i].technique == 'n')
		{
			instructions[i].technique = 'c';
			instructions[i].number = deckSize-instructions[i].number;
		}
	}

	unsigned iPtr = 1;
	while(instructions.size() > 2)
	{
		if(instructions[iPtr].technique == instructions[iPtr-1].technique)
		{
			if(instructions[iPtr].technique == 'd')
			{
				instructions[iPtr-1].number *= instructions[iPtr].number;
				instructions[iPtr-1].number %= deckSize;
			}
			else
			{
				instructions[iPtr-1].number += instructions[iPtr].number;
				instructions[iPtr-1].number %= deckSize;
			}
			instructions.erase(instructions.begin()+iPtr);
		}
		else
		{
			if(instructions[iPtr].technique == 'd')
			{
				Instruction temp = instructions[iPtr-1];
				instructions[iPtr-1] = instructions[iPtr];
				instructions[iPtr] = temp;
				instructions[iPtr].number *= instructions[iPtr-1].number;
				instructions[iPtr].number %= deckSize;
				if(iPtr > 1)
				{
					iPtr = 1;
				}
			}
			else
			{
				iPtr++;
			}
		}
	}
}

uint64_t shuffle(std::vector<Instruction> instructions)
{
	uint64_t result = 0;
	
	boost::multiprecision::cpp_int top = 0;
	boost::multiprecision::cpp_int stepSize = 1;
	boost::multiprecision::cpp_int deckSize = 10007;
	uint64_t iterations = 1;
	
	std::bitset<64> bits(iterations);
	collapseInstructions(instructions,deckSize);

	for(auto& instruction:instructions)
	{
		switch(instruction.technique)
		{
			case 'd':
			{
				stepSize = (stepSize*instruction.number)%deckSize;
				break;
			}
			case 'c':
			{
				boost::multiprecision::cpp_int multiplier = inverseMod(stepSize, deckSize);
				top = (instruction.number/stepSize) + (instruction.number%stepSize)*multiplier;
				break;
			}
		}
	}

	boost::multiprecision::cpp_int step = inverseMod(stepSize,deckSize);

	
	while(top != 2019)
	{
		top = (top+step)%deckSize;
		result++;
	}

	return result;
}

boost::multiprecision::cpp_int bigShuffle(std::vector<Instruction> &instructions)
{
	boost::multiprecision::cpp_int result = 0;
	
	boost::multiprecision::cpp_int top = 0;
	boost::multiprecision::cpp_int stepSize = 1;
	boost::multiprecision::cpp_int deckSize = 119315717514047;
	uint64_t iterations = 101741582076661;
	
	std::bitset<64> bits(iterations);
	collapseInstructions(instructions,deckSize);
	std::unordered_map<uint64_t,std::vector<Instruction>> partialInstructions;
	uint64_t multiplier = 1;
	
	while(multiplier <= iterations)
	{
		partialInstructions[multiplier] = instructions;
		instructions.push_back(instructions[0]);
		instructions.push_back(instructions[1]);
		collapseInstructions(instructions,deckSize);
		multiplier *= 2;
	}
	instructions.clear();
	multiplier = 1;
	for(unsigned i=0; i<64; i++)
	{
		if(bits.test(i) == 1)
		{
			instructions.push_back(partialInstructions[multiplier][0]);
			instructions.push_back(partialInstructions[multiplier][1]);
		}
		multiplier *=2;
	}
	
	collapseInstructions(instructions,deckSize);

	for(auto& instruction:instructions)
	{
		switch(instruction.technique)
		{
			case 'd':
			{
				stepSize = (stepSize*instruction.number)%deckSize;
				break;
			}
			case 'c':
			{
				boost::multiprecision::cpp_int multiplier = inverseMod(stepSize, deckSize);
				top = (instruction.number/stepSize) + (instruction.number%stepSize)*multiplier;
				break;
			}
		}
	}

	boost::multiprecision::cpp_int step = inverseMod(stepSize,deckSize);

	top = (top+step*2020)%deckSize;
	
	result = top;

	return result;
}

int main()
{
	uint64_t resultA = 0;
	boost::multiprecision::cpp_int resultB = 0;
	
	std::vector<Instruction> instructions;
   
  parseInput(instructions);
	
 	resultA = shuffle(instructions);
	resultB = bigShuffle(instructions);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
 	
  return 0;
}
