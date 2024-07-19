#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

void parseInput(std::vector<unsigned> &modules)
{
  std::string line;
  
  std::ifstream input("input.txt");

  if(input.is_open())
  {
    while(getline(input,line))
    {
			modules.push_back(std::stoi(line));
    }
  }
  input.close();
}

uint64_t calculateFuelNeeds(const std::vector<unsigned> &modules)
{
	uint64_t result = 0;
	
	for(auto mass : modules)
	{
		result += (mass/3)-2;
	}
	
	return result;
}

uint64_t calculateRealFuelNeeds(const std::vector<unsigned> &modules)
{
	uint64_t result = 0;
	
	int fuelMass = 0;
	
	for(auto mass : modules)
	{
		fuelMass = (mass/3)-2;
		
		while(fuelMass > 0)
		{
			result += fuelMass;
			fuelMass = (fuelMass/3)-2;
		}
	}
	
	return result;
}

int main()
{
  uint64_t resultA = 0;
  uint64_t resultB = 0;
  
	std::vector<unsigned> modules;
   
  parseInput(modules);

 	resultA = calculateFuelNeeds(modules);
 	resultB = calculateRealFuelNeeds(modules);
 
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
  return 0;
}
