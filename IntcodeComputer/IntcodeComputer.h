#ifndef INTCODE_COMPUTER_H
#define INTCODE_COMPUTER_H

#include <vector>
#include <array>
#include <queue>

class IntcodeComputer
{
	typedef void(IntcodeComputer::*opCode)(int, int, int, std::vector<int> &program, int&);

	void add(int a, int b, int c, std::vector<int> &program, int &adressPtr);

	void mul(int a, int b, int c, std::vector<int> &program, int &adressPtr);

	void in(int a, std::vector<int> &program, int &adressPtr);

	void in(int a, int b, int c, std::vector<int> &program, int &adressPtr);

	void out(int a, std::vector<int> &program, int &adressPtr);

	void out(int a, int b, int c, std::vector<int> &program, int &adressPtr);

	void jit(int a, int b, std::vector<int> &program, int &adressPtr);

	void jit(int a, int b, int c, std::vector<int> &program, int &adressPtr);

	void jif(int a, int b, std::vector<int> &program, int &adressPtr);

	void jif(int a, int b, int c, std::vector<int> &program, int &adressPtr);

	void les(int a, int b, int c, std::vector<int> &program, int &adressPtr);

	void equ(int a, int b, int c, std::vector<int> &program, int &adressPtr);

	void initializeOpCodes(std::array<opCode,8> &opCodes);

	unsigned determineMode(int op, int position);

	void loadValues(int op, int adress, int &a, int &b, int &c, std::vector<int> &program);

	std::array<opCode, 8> opCodes;
	int adressPtr = 0;
	bool opCodesInitialized = false;
	
	public:
	
	IntcodeComputer();
	
	void executeProgram();
	void loadProgram(std::vector<int> newProgram);
	
	std::queue<int> inbox;
	std::queue<int> outbox;
	std::vector<int> program;
	bool sleeping = false;
};

#endif
