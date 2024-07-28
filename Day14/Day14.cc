#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

struct Chemical
{
	std::string name;
	uint64_t amount = 0;
	uint64_t yield = 0;
	uint64_t level = 0;
	std::vector<std::pair<std::string,uint64_t>> reactants;
};

void parseInput(std::vector<Chemical> &chemicals)
{
  std::string line;
  
  size_t pos = 0;
  size_t endpos = 0;
  uint64_t yield = 0;
  std::string name;
 	
  std::ifstream input("input.txt");
  
  Chemical chemical;
  chemical.name = "ORE";
	chemical.yield = 1000000000;
	chemicals.push_back(chemical);
	
  if(input.is_open())
  {	
  	while(getline(input,line))
  	{
  		std::vector<std::pair<std::string,uint64_t>> reactants;
  		pos = line.find_first_of(">1234567890");
			while(line[pos] != '>')
			{
				endpos = line.find_first_of(" ", pos);
				yield = std::stoi(line.substr(pos,endpos-pos));
				pos = endpos + 1;
				endpos = line.find_first_of(", ", pos);
				name = line.substr(pos,endpos-pos);
				reactants.push_back(std::make_pair(name,yield));
  			pos = line.find_first_of(">1234567890", endpos);
			}
			
		 	Chemical chemical;
		 	
			pos += 2;	
			endpos = line.find_first_of(" ", pos);
			yield = std::stoi(line.substr(pos,endpos-pos));
			pos = endpos + 1;
			name = line.substr(pos);
			
			chemical.yield = yield;
			chemical.name = name;
			chemical.reactants = reactants;
			
			if(name == "FUEL")
			{
				chemical.amount = 1;
			}
			
			chemicals.push_back(chemical);
  	}  	
  }
  input.close();
}

void orderChemicals(std::vector<Chemical> &chemicals, const std::string &reactantName, unsigned level)
{
	for(auto& chemical:chemicals)
	{
		for(auto& reactant:chemical.reactants)
		{
			if(reactantName == reactant.first && level > chemical.level)
			{
				chemical.level = level;
				orderChemicals(chemicals, chemical.name, level+1);
			}
		}
	}
}

void orderChemicals(std::vector<Chemical> &chemicals)
{
	std::string reactantName = "ORE";
	unsigned level = 1;
	
	orderChemicals(chemicals, reactantName, level);
}

uint64_t convertORE(std::vector<Chemical> chemicals)
{
	uint64_t result = 0;
	
	bool done = false;
	
	while(!done)
	{
		done = true;
		
		bool changed = true;
		
		while(changed)
		{
			changed = false;
			for(auto& chemical : chemicals)
			{
				uint64_t multiple = 0;
				if(chemical.name != "ORE" && chemical.amount >= chemical.yield)
				{
					changed = true;
					multiple = chemical.amount / chemical.yield;
					chemical.amount %= chemical.yield;
					for(auto& reactant:chemical.reactants)
					{
						for(auto& reactantChem:chemicals)
						{
							if(reactantChem.name == reactant.first)
							{
								reactantChem.amount += reactant.second*multiple;
								break;
							}
						}
					}
				}
			}
		}
		
		unsigned maxLevel = 0;
		std::string maxName;
		for(auto& chemical : chemicals)
		{
			if(maxLevel < chemical.level && chemical.amount > 0)
			{
				done = false;
				maxName = chemical.name;
				maxLevel = chemical.level;
			}
		}
		
		if(!done)
		{
			for(auto& chemical : chemicals)
			{
				if(maxName == chemical.name)
				{
					chemical.amount = chemical.yield;
				}
			}
		}
	}

	for(auto& chemical:chemicals)
	{
		if(chemical.name == "ORE")
		{
			result = chemical.amount;
		}
	}

	return result;
}

uint64_t oneTrillionORE(std::vector<Chemical> &baseChemicals)
{
	uint64_t result = 0;
	
	unsigned fuelAmount = 1000000;
	Chemical* fuelChemical = nullptr;
	
	for(auto& chemical:baseChemicals)
	{
		if(chemical.name == "FUEL")
		{
			fuelChemical = &chemical;
		}
	}
	
	while(fuelChemical != nullptr && fuelAmount != 0)
	{
		fuelChemical->amount += fuelAmount;
		auto chemicals = baseChemicals;
		
		bool changed = true;
		
		while(changed)
		{
			changed = false;
			for(auto& chemical : chemicals)
			{
				uint64_t multiple = 0;
				if(chemical.name != "ORE" && chemical.amount >= chemical.yield)
				{
					changed = true;
					multiple = chemical.amount / chemical.yield;
					chemical.amount %= chemical.yield;
					for(auto& reactant:chemical.reactants)
					{
						for(auto& reactantChem:chemicals)
						{
							if(reactantChem.name == reactant.first)
							{
								reactantChem.amount += reactant.second*multiple;
								break;
							}
						}
					}
				}
			}
		}
		
		for(auto& chemical:chemicals)
		{
			if(chemical.name == "ORE")
			{
				if(chemical.amount > 1000000000000)
				{
					fuelChemical->amount -= fuelAmount;
					fuelAmount /= 10;
				}
				break;
			}
		}
	}

	result = fuelChemical->amount;
	
	return result;
}


int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<Chemical> chemicals;
   
  parseInput(chemicals);
	
	orderChemicals(chemicals);
	
 	resultA = convertORE(chemicals);
	resultB = oneTrillionORE(chemicals);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
 	
  return 0;
}
