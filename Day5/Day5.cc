#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <array>

typedef void(*opCode)(int, int, int, std::vector<int> &code, int&);

void parseInput(std::vector<int> &code)
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
			code.push_back(std::stoi(line.substr(pos,endpos-pos)));
  		pos = line.find_first_of("-1234567890",endpos);
		}
  }
  input.close();
}

void add(int a, int b, int c, std::vector<int> &code, int &adressPtr)
{
	code[c] = code[a]+code[b];
	adressPtr += 4;
}

void mul(int a, int b, int c, std::vector<int> &code, int &adressPtr)
{
	code[c] = code[a]*code[b];
	adressPtr += 4;
}

void in(int a, std::vector<int> &code, int &adressPtr)
{	
	std::cout << "Please enter ID of the system to test: ";
	std::cin >> code[a];
	std::cout << std::endl;
	adressPtr += 2;
}

void in(int a, int b, int c, std::vector<int> &code, int &adressPtr)
{
	in(a, code, adressPtr);
}

void out(int a, std::vector<int> &code, int &adressPtr)
{
	std::cout << "output: " << code[a] << std::endl;
	adressPtr += 2;
}

void out(int a, int b, int c, std::vector<int> &code, int &adressPtr)
{
	out(a, code, adressPtr);
}

void jit(int a, int b, std::vector<int> &code, int &adressPtr)
{
	adressPtr = (code[a] != 0) ? code[b] : adressPtr+3;
}

void jit(int a, int b, int c, std::vector<int> &code, int &adressPtr)
{
	jit(a, b, code, adressPtr);
}

void jif(int a, int b, std::vector<int> &code, int &adressPtr)
{
	adressPtr = (code[a] == 0) ? code[b] : adressPtr+3;
}

void jif(int a, int b, int c, std::vector<int> &code, int &adressPtr)
{
	jif(a, b, code, adressPtr);
}

void les(int a, int b, int c, std::vector<int> &code, int &adressPtr)
{
	code[c] = (code[a] < code[b]) ? 1 : 0;
	adressPtr += 4;
}

void equ(int a, int b, int c, std::vector<int> &code, int &adressPtr)
{
	code[c] = (code[a] == code[b]) ? 1 : 0;
	adressPtr += 4;
}

void initializeOpCodes(std::array<opCode,8> &opCodes)
{
	opCodes[0] = &add;
	opCodes[1] = &mul;
	opCodes[2] = &in;
	opCodes[3] = &out;
	opCodes[4] = &jit;
	opCodes[5] = &jif;
	opCodes[6] = &les;
	opCodes[7] = &equ;
}

unsigned determineMode(int op, int position)
{
	unsigned result = 0;
	
	if((op / position)%10 == 1)
	{	
		result = 1;
	}

	return result;
}

void loadValues(int op, int adress, int &a, int &b, int &c, std::vector<int> &code)
{
	if(abs(adress+1)<code.size())
	{
		switch(determineMode(op, 100))
		{
			case 0:
			{
				a = code[adress+1];
				break;
			}
			case 1:
			{
				a = adress+1;
				break;
			}
		}
	}

	if(abs(adress+2)<code.size())
	{
		switch(determineMode(op, 1000))
		{
			case 0:
			{
				b = code[adress+2];
				break;
			}
			case 1:
			{
				b = adress+2;
				break;
			}
		}
	}

	if(abs(adress+3)<code.size())
	{
		switch(determineMode(op, 10000))
		{
			case 0:
			{
				c = code[adress+3];
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

void executeProgram(std::vector<int> &code)
{
	std::array<opCode, 8> opCodes;
	initializeOpCodes(opCodes);
	
	int a = 0;
	int b = 0;
	int c = 0;
	int adressPtr = 0;
	
	while(adressPtr >= 0 && abs(adressPtr) < code.size())
	{
		if(code[adressPtr]%100 == 99)
		{
			break;
		}
		loadValues(code[adressPtr], adressPtr, a, b, c, code);
		opCodes[(code[adressPtr]%100)-1](a, b, c, code, adressPtr);
	}
}

int main()
{
	std::vector<int> code;
  parseInput(code);

 	executeProgram(code);

  return 0;
}
