#include <iostream>
#include <fstream>
#include <vector>

void parseInput(unsigned &from, unsigned &to)
{
  std::string line;
  
  std::ifstream input("input.txt");

  if(input.is_open())
  {
  	getline(input,line);
  	size_t pos = line.find_first_of("-");
  	from = std::stoi(line.substr(0,pos));
  	to = std::stoi(line.substr(pos+1));
  }
  input.close();
}

uint64_t numberOfPasswords(unsigned from, unsigned to)
{
	uint64_t result = 0;
	
	for(unsigned i=from; i<=to; i++)
	{
		bool adjacent = false;
		bool increasing = true;
		std::string str = std::to_string(i);
		for(unsigned j=0; j<str.size()-1; j++)
		{
			if(str[j] > str[j+1])
			{
				increasing = false;
			}
			if(str[j] == str[j+1])
			{
				adjacent = true;
			}
		}
		if(adjacent && increasing)
		{
			result++;
		}
	}
	
	return result;
}

uint64_t newRule(unsigned from, unsigned to)
{
	uint64_t result = 0;
	
	for(unsigned i=from; i<=to; i++)
	{
		bool adjacent = false;
		bool increasing = true;
		std::string str = std::to_string(i);
		for(unsigned j=0; j<str.size()-1; j++)
		{
			if(str[j] > str[j+1])
			{
				increasing = false;
			}
			if(str[j] == str[j+1])
			{
				if(j>0 && j+2<str.size())
				{
					if(str[j] != str[j+2] && str[j] != str[j-1])
					{
						adjacent = true;
					}
				}
				else
				{
					if(j+2<str.size() && str[j] != str[j+2])
					{
						adjacent = true;
					}
					else if(j>0 && str[j] != str[j-1])
					{
						adjacent = true;
					}
				}
			}
		}
		if(adjacent && increasing)
		{
			result++;
		}
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
  unsigned from = 0;
  unsigned to = 0;
   
  parseInput(from, to);

 	resultA = numberOfPasswords(from, to);
 	resultB = newRule(from, to);
 
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
  return 0;
}
