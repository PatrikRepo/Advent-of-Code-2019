#include "IntcodeComputer.h"
#include <iostream>

IntcodeComputer::IntcodeComputer()
{
	initializeOpCodes(opCodes);
}

void IntcodeComputer::add(int a, int b, int c, std::vector<int> &program, int &adressPtr)
{
	program[c] = program[a]+program[b];
	adressPtr += 4;
}

void IntcodeComputer::mul(int a, int b, int c, std::vector<int> &program, int &adressPtr)
{
	program[c] = program[a]*program[b];
	adressPtr += 4;
}

void IntcodeComputer::in(int a, std::vector<int> &program, int &adressPtr)
{	
	if(!inbox.empty())
	{
		program[a] = inbox.front();
		inbox.pop();
		adressPtr += 2;
	}
	else
	{
		sleeping = true;
	}
}

void IntcodeComputer::in(int a, int b, int c, std::vector<int> &program, int &adressPtr)
{
	in(a, program, adressPtr);
}

void IntcodeComputer::out(int a, std::vector<int> &program, int &adressPtr)
{
	outbox.push(program[a]);
	adressPtr += 2;
}

void IntcodeComputer::out(int a, int b, int c, std::vector<int> &program, int &adressPtr)
{
	out(a, program, adressPtr);
}

void IntcodeComputer::jit(int a, int b, std::vector<int> &program, int &adressPtr)
{
	adressPtr = (program[a] != 0) ? program[b] : adressPtr+3;
}

void IntcodeComputer::jit(int a, int b, int c, std::vector<int> &program, int &adressPtr)
{
	jit(a, b, program, adressPtr);
}

void IntcodeComputer::jif(int a, int b, std::vector<int> &program, int &adressPtr)
{
	adressPtr = (program[a] == 0) ? program[b] : adressPtr+3;
}

void IntcodeComputer::jif(int a, int b, int c, std::vector<int> &program, int &adressPtr)
{
	jif(a, b, program, adressPtr);
}

void IntcodeComputer::les(int a, int b, int c, std::vector<int> &program, int &adressPtr)
{
	program[c] = (program[a] < program[b]) ? 1 : 0;
	adressPtr += 4;
}

void IntcodeComputer::equ(int a, int b, int c, std::vector<int> &program, int &adressPtr)
{
	program[c] = (program[a] == program[b]) ? 1 : 0;
	adressPtr += 4;
}

void IntcodeComputer::initializeOpCodes(std::array<IntcodeComputer::opCode,8> &opCodes)
{
	opCodes[0] = &IntcodeComputer::add;
	opCodes[1] = &IntcodeComputer::mul;
	opCodes[2] = &IntcodeComputer::in;
	opCodes[3] = &IntcodeComputer::out;
	opCodes[4] = &IntcodeComputer::jit;
	opCodes[5] = &IntcodeComputer::jif;
	opCodes[6] = &IntcodeComputer::les;
	opCodes[7] = &IntcodeComputer::equ;
}

unsigned IntcodeComputer::determineMode(int op, int position)
{
	unsigned result = 0;
	
	if((op / position)%10 == 1)
	{	
		result = 1;
	}

	return result;
}

void IntcodeComputer::loadValues(int op, int adress, int &a, int &b, int &c, std::vector<int> &program)
{
	if(abs(adress+1)<program.size())
	{
		switch(determineMode(op, 100))
		{
			case 0:
			{
				a = program[adress+1];
				break;
			}
			case 1:
			{
				a = adress+1;
				break;
			}
		}
	}

	if(abs(adress+2)<program.size())
	{
		switch(determineMode(op, 1000))
		{
			case 0:
			{
				b = program[adress+2];
				break;
			}
			case 1:
			{
				b = adress+2;
				break;
			}
		}
	}

	if(abs(adress+3)<program.size())
	{
		switch(determineMode(op, 10000))
		{
			case 0:
			{
				c = program[adress+3];
				break;
			}
			case 1:
			{
				c = adress+3;
				break;
			}
		}
	}
}

void IntcodeComputer::executeProgram()
{
	sleeping = false;
	
	int a = 0;
	int b = 0;
	int c = 0;
	
	while(!sleeping && adressPtr >= 0 && abs(adressPtr) < program.size())
	{
		if(program[adressPtr]%100 == 99)
		{
			break;
		}
		loadValues(program[adressPtr], adressPtr, a, b, c, program);
		(this->*opCodes[(program[adressPtr]%100)-1])(a, b, c, program, adressPtr);
	}
}

void IntcodeComputer::loadProgram(std::vector<int> newProgram)
{
	program = newProgram;
}
