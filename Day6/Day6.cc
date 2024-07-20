#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

struct Body
{
	std::string orbits;
	std::vector<std::string> satellites;
	unsigned orbitNumber = 0;
};

void parseInput(std::unordered_map<std::string,Body> &bodies)
{
  std::string line;
  
  std::ifstream input("input.txt");

  if(input.is_open())
  {
  	Body* body;
  	Body* satellite;
  	std::string bodyName;
  	std::string satelliteName;
  	while(getline(input,line))
  	{
  		bodyName = line.substr(0,3);
  		satelliteName = line.substr(4);
  		if(bodies.count(bodyName) == 0)
  		{
  			bodies[bodyName] = Body();
  		}
  		body = &bodies[bodyName];
  		
  		if(bodies.count(satelliteName) == 0)
  		{
  			bodies[satelliteName] = Body();
  		}
  		satellite = &bodies[satelliteName];
  		
  		body->satellites.push_back(satelliteName);
  		satellite->orbits = bodyName;
  	}
  }
  input.close();
}

void assignOrbitNumber(Body &body, std::unordered_map<std::string, Body> &bodies)
{
	for(auto &satellite : body.satellites)
	{
		bodies[satellite].orbitNumber = body.orbitNumber + 1; 
		assignOrbitNumber(bodies[satellite], bodies);
	}
}

uint64_t countOrbits(std::unordered_map<std::string,Body> &bodies)
{
	uint64_t result = 0;
	
	for(auto &body : bodies)
	{
		result += body.second.orbitNumber;
	}
	
	return result;
}

uint64_t countTransfers(std::unordered_map<std::string,Body> &bodies)
{
	uint64_t result = 0;
	
	unsigned steps = 0;
	
	std::string youNode = bodies["YOU"].orbits;
	std::string santaNode = bodies["SAN"].orbits;
	unsigned youOrbitNumber = bodies[youNode].orbitNumber;
	unsigned santaOrbitNumber = bodies[santaNode].orbitNumber;

	if(youOrbitNumber > santaOrbitNumber)
	{
		for(unsigned i=youOrbitNumber; i>santaOrbitNumber; i--)
		{
			steps++;
			youNode = bodies[youNode].orbits;
			youOrbitNumber--;
		}
	}
	else
	{
		for(unsigned i=santaOrbitNumber; i>youOrbitNumber; i--)
		{
			steps++;
			santaNode = bodies[santaNode].orbits;
			santaOrbitNumber--;
		}
	}
	
	while(santaNode != youNode)
	{
		steps += 2;
		youNode = bodies[youNode].orbits;
		santaNode = bodies[santaNode].orbits;
	}
	result = steps;
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
  std::unordered_map<std::string,Body> bodies;
   
  parseInput(bodies);

	assignOrbitNumber(bodies["COM"], bodies);
	
 	resultA = countOrbits(bodies);
 	resultB = countTransfers(bodies);
 
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
  return 0;
}
