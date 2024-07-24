#include "IntcodeComputer.h"
#include <iostream>

IntcodeComputer::IntcodeComputer()
{
	initializeOpCodes(opCodes);
	adressPtr = 0;
	offset = 0;
}

void IntcodeComputer::add(int64_t a, int64_t b, int64_t c, std::array<int64_t, 10000> &program, int64_t &adressPtr)
{
	program[c] = program[a]+program[b];
	adressPtr += 4;
}

void IntcodeComputer::mul(int64_t a, int64_t b, int64_t c, std::array<int64_t, 10000> &program, int64_t &adressPtr)
{
	program[c] = program[a]*program[b];
	adressPtr += 4;
}

void IntcodeComputer::in(int64_t a, std::array<int64_t, 10000> &program, int64_t &adressPtr)
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

void IntcodeComputer::in(int64_t a, int64_t b, int64_t c, std::array<int64_t, 10000> &program, int64_t &adressPtr)
{
	in(a, program, adressPtr);
}

void IntcodeComputer::out(int64_t a, std::array<int64_t, 10000> &program, int64_t &adressPtr)
{
	outbox.push(program[a]);
	adressPtr += 2;
}

void IntcodeComputer::out(int64_t a, int64_t b, int64_t c, std::array<int64_t, 10000> &program, int64_t &adressPtr)
{
	out(a, program, adressPtr);
}

void IntcodeComputer::jit(int64_t a, int64_t b, std::array<int64_t, 10000> &program, int64_t &adressPtr)
{
	adressPtr = (program[a] != 0) ? program[b] : adressPtr+3;
}

void IntcodeComputer::jit(int64_t a, int64_t b, int64_t c, std::array<int64_t, 10000> &program, int64_t &adressPtr)
{
	jit(a, b, program, adressPtr);
}

void IntcodeComputer::jif(int64_t a, int64_t b, std::array<int64_t, 10000> &program, int64_t &adressPtr)
{
	adressPtr = (program[a] == 0) ? program[b] : adressPtr+3;
}

void IntcodeComputer::jif(int64_t a, int64_t b, int64_t c, std::array<int64_t, 10000> &program, int64_t &adressPtr)
{
	jif(a, b, program, adressPtr);
}

void IntcodeComputer::les(int64_t a, int64_t b, int64_t c, std::array<int64_t, 10000> &program, int64_t &adressPtr)
{
	program[c] = (program[a] < program[b]) ? 1 : 0;
	adressPtr += 4;
}

void IntcodeComputer::equ(int64_t a, int64_t b, int64_t c, std::array<int64_t, 10000> &program, int64_t &adressPtr)
{
	program[c] = (program[a] == program[b]) ? 1 : 0;
	adressPtr += 4;
}

void IntcodeComputer::adj(int64_t a, std::array<int64_t, 10000> &program, int64_t &adressPtr)
{
	offset += program[a];
	adressPtr += 2;
}

void IntcodeComputer::adj(int64_t a, int64_t b, int64_t c, std::array<int64_t, 10000> &program, int64_t &adressPtr)
{
	adj(a, program, adressPtr);
}

void IntcodeComputer::initializeOpCodes(std::array<IntcodeComputer::opCode,9> &opCodes)
{
	opCodes[0] = &IntcodeComputer::add;
	opCodes[1] = &IntcodeComputer::mul;
	opCodes[2] = &IntcodeComputer::in;
	opCodes[3] = &IntcodeComputer::out;
	opCodes[4] = &IntcodeComputer::jit;
	opCodes[5] = &IntcodeComputer::jif;
	opCodes[6] = &IntcodeComputer::les;
	opCodes[7] = &IntcodeComputer::equ;
	opCodes[8] = &IntcodeComputer::adj;
}

unsigned IntcodeComputer::determineMode(int64_t op, int64_t position)
{
	unsigned result = 0;
	
	int value = (op/position)%10;
	if(value == 1)
	{	
		result = 1;
	}
	else if(value == 2)
	{	
		result = 2;
	}

	return result;
}

void IntcodeComputer::loadValues(int64_t op, int64_t adress, int64_t &a, int64_t &b, int64_t &c, std::array<int64_t, 10000> &program)
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
			case 2:
			{
				a = program[adress+1]+offset;
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
			case 2:
			{
				b = program[adress+2]+offset;
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
			case 2:
			{
				c = program[adress+3]+offset;
				break;
			}
		}
	}
}

void IntcodeComputer::pushValue(int64_t value)
{
	inbox.push(value);
}

int64_t IntcodeComputer::fetchValue()
{
	int64_t result = outbox.front();
	outbox.pop();
	return result;
}

bool IntcodeComputer::isSleeping()
{
	return sleeping;
}

void IntcodeComputer::executeProgram()
{
	sleeping = false;
	
	int64_t a = 0;
	int64_t b = 0;
	int64_t c = 0;
	
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

void IntcodeComputer::loadProgram(const std::vector<int64_t> &newProgram)
{
	for(unsigned i=0; i<newProgram.size(); i++)
	{
		program[i] = newProgram[i];
	}
}
