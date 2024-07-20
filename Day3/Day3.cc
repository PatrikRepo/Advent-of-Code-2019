#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

struct Line
{
	char direction = 0;
	int xMin = 0;
	int xMax = 0;
	int yMin = 0;
	int yMax = 0;
};

void parseInput(std::vector<std::vector<Line>> &wires)
{
  std::string line;
  
  std::ifstream input("input.txt");

  if(input.is_open())
  {
		while(getline(input,line))
		{
		  size_t pos = 0;
			size_t endpos = 0;
			Line wireLine;
			std::vector<Line> wire;
			int x = 0;
			int y = 0;
			int nextX = 0;
			int nextY = 0;
			int steps = 0;
			
		  while(pos != std::string::npos)
		  {
		  	endpos = line.find_first_of(",",pos);
		  	wireLine.direction = line[pos];
				steps = endpos != std::string::npos ? std::stoi(line.substr(pos+1,endpos-pos-1)) : std::stoi(line.substr(pos+1));
				switch(line[pos])
				{
					case 'U':
					{
						nextY -= steps;
						break;
					}
					case 'R':
					{
						nextX += steps;
						break;
					}
					case 'D':
					{
						nextY += steps;
						break;
					}
					case 'L':
					{	
						nextX -= steps;
					}
				}
				wireLine.xMin = (x < nextX) ? x : nextX;
				wireLine.xMax = (x > nextX) ? x : nextX;
				wireLine.yMin = (y < nextY) ? y : nextY;
				wireLine.yMax = (y > nextY) ? y : nextY;

				pos = (endpos != std::string::npos) ? line.find_first_of("URDL",endpos) : std::string::npos;
				wire.push_back(wireLine);
				x = nextX;
				y = nextY;
		  }
		  wires.push_back(wire);
		}
  }
  input.close();
}

std::pair<uint64_t,uint64_t> closestCrossing(const std::vector<std::vector<Line>> &wires)
{
	std::pair<uint64_t,uint64_t> result{UINT64_MAX, UINT64_MAX};
	
	unsigned wire1Steps = 0;
	for(auto &wire1 : wires[0])
	{
		unsigned wire2Steps = 0;
		for(auto &wire2 : wires[1])
		{
			if(wire1.xMin != wire2.xMax &&
				wire2.yMin != wire2.yMax && 
				wire2.xMin > wire1.xMin && 
				wire2.xMin < wire1.xMax && 
				wire2.yMin < wire1.yMin && 
				wire2.yMax > wire1.yMin)
			{
				unsigned dist = abs(wire1.yMin) + abs(wire2.xMin);
				result.first = (dist < result.first) ? dist : result.first;
				
				unsigned steps = wire1Steps + wire2Steps;
				steps += wire1.direction == 'R' ? abs(wire1.xMin - wire2.xMin) : abs(wire1.xMax - wire2.xMin); 
				steps += wire2.direction == 'D' ? abs(wire2.yMin - wire1.yMin) : abs(wire2.yMax - wire1.yMin); 
				result.second = (steps < result.second) ? steps : result.second;
			}
			else if(wire1.yMin != wire2.yMax && 
				wire2.xMin != wire2.xMax && 
				wire2.yMin > wire1.yMin && 
				wire2.yMin < wire1.yMax && 
				wire2.xMin < wire1.xMin && 
				wire2.xMax > wire1.xMin)
			{
				unsigned dist = abs(wire1.xMin) + abs(wire2.yMin);
				result.first = (dist < result.first) ? dist : result.first;
				
				unsigned steps = wire1Steps + wire2Steps;
				steps += wire2.direction == 'R' ? abs(wire2.xMin - wire1.xMin) : abs(wire2.xMax - wire1.xMin); 
				steps += wire1.direction == 'D' ? abs(wire1.yMin - wire2.yMin) : abs(wire1.yMax - wire2.yMin); 
				result.second = (steps < result.second) ? steps : result.second;
			}
			wire2Steps += abs(wire2.xMax - wire2.xMin) + abs(wire2.yMax - wire2.yMin);
		}
		wire1Steps += abs(wire1.xMax - wire1.xMin) + abs(wire1.yMax - wire1.yMin);	
	}
	
	return result;
}

int main()
{
  std::pair<uint64_t,uint64_t> result;
  
	std::vector<std::vector<Line>> wires;
   
  parseInput(wires);

 	result = closestCrossing(wires);
 
  std::cout << "resultA: " << result.first << '\n';
  std::cout << "resultB: " << result.second << std::endl;
  return 0;
}
