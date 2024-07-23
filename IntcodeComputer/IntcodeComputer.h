#ifndef INTCODE_COMPUTER_H
#define INTCODE_COMPUTER_H

#include <vector>
#include <array>
#include <queue>

class IntcodeComputer
{
	typedef void(IntcodeComputer::*opCode)(int64_t, int64_t, int64_t, std::array<int64_t, 10000> &program, int64_t&);

	void add(int64_t a, int64_t b, int64_t c, std::array<int64_t, 10000> &program, int64_t &adressPtr);

	void mul(int64_t a, int64_t b, int64_t c, std::array<int64_t, 10000> &program, int64_t &adressPtr);

	void in(int64_t a, std::array<int64_t, 10000> &program, int64_t &adressPtr);

	void in(int64_t a, int64_t b, int64_t c, std::array<int64_t, 10000> &program, int64_t &adressPtr);

	void out(int64_t a, std::array<int64_t, 10000> &program, int64_t &adressPtr);

	void out(int64_t a, int64_t b, int64_t c, std::array<int64_t, 10000> &program, int64_t &adressPtr);

	void jit(int64_t a, int64_t b, std::array<int64_t, 10000> &program, int64_t &adressPtr);

	void jit(int64_t a, int64_t b, int64_t c, std::array<int64_t, 10000> &program, int64_t &adressPtr);

	void jif(int64_t a, int64_t b, std::array<int64_t, 10000> &program, int64_t &adressPtr);

	void jif(int64_t a, int64_t b, int64_t c, std::array<int64_t, 10000> &program, int64_t &adressPtr);

	void les(int64_t a, int64_t b, int64_t c, std::array<int64_t, 10000> &program, int64_t &adressPtr);

	void equ(int64_t a, int64_t b, int64_t c, std::array<int64_t, 10000> &program, int64_t &adressPtr);

	void adj(int64_t a, std::array<int64_t, 10000> &program, int64_t &adressPtr);

	void adj(int64_t a, int64_t b, int64_t c, std::array<int64_t, 10000> &program, int64_t &adressPtr);
	

	void initializeOpCodes(std::array<opCode,9> &opCodes);

	unsigned determineMode(int64_t op, int64_t position);

	void loadValues(int64_t op, int64_t adress, int64_t &a, int64_t &b, int64_t &c, std::array<int64_t, 10000> &program);

	std::array<opCode,9> opCodes;
	int64_t adressPtr = 0;
	int64_t offset = 0;
	bool opCodesInitialized = false;
	
	public:
	
	IntcodeComputer();
	
	void executeProgram();
	void loadProgram(const std::vector<int64_t> &newProgram);
	
	std::queue<int64_t> inbox;
	std::queue<int64_t> outbox;
	std::array<int64_t, 10000> program{0};
	bool sleeping = false;
};

#endif
