#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

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

uint64_t executeProgram(std::vector<int> &code)
{
	uint64_t result = 0;
	
	for(unsigned i=0; i<code.size()-3; i+=4)
	{
		switch(code[i])
		{
			case 1:
			{
				code[code[i+3]] = code[code[i+1]]+code[code[i+2]];
				break;
			}
			case 2:
			{
				code[code[i+3]] = code[code[i+1]]*code[code[i+2]];
				break;
			}
			case 99:
			{
				i = code.size();
				break;
			}
			default:
			{
				std::cout << "ERROR: Opcode is: " << code[i] << std::endl;
			}
		}
	}
	
	result = code[0];
	
	return result;
}

std::pair<uint64_t,uint64_t> findValues(std::vector<int> &baseCode)
{
	std::pair<uint64_t,uint64_t> result;
	
	auto code = baseCode;
	code[1] = 12;
	code[2] = 2;
	result.first = executeProgram(code);
	
	for(unsigned i=0; i<=99; i++)
	{
		for(unsigned j=0; j<=99; j++)
		{
			code = baseCode;
			code[1] = i;
			code[2] = j;
			if(executeProgram(code) == 19690720)
			{
				result.second = i*100 + j;
				i=100;
				break;
			}
		}
	}
	
	return result;
}

int main()
{
  std::pair<uint64_t,uint64_t> result;
  
	std::vector<int> code;
  parseInput(code);

 	result = findValues(code);
 
  std::cout << "resultA: " << result.first << '\n';
  std::cout << "resultB: " << result.second << std::endl;
  return 0;
}
