#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <unordered_map>

void parseInput(std::array<std::string,5> &map)
{
	std::string line;
	
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
  	unsigned i = 0;
  	while(getline(input, line))
  	{
  		map[i] = line;
  		i++;
  	}
  }
  input.close();
}

unsigned countBugs(const std::array<std::string,5> &map, size_t x, size_t y)
{
	unsigned result = 0;
	
	if(y>0 && map[y-1][x] == '#')
	{
		result++;
	}
	if(x>0 && map[y][x-1] == '#')
	{
		result++;
	}
	if(x+1<map[0].size() && map[y][x+1] == '#')
	{
		result++;
	}
	if(y+1<map.size() && map[y+1][x] == '#')
	{
		result++;
	}
	
	return result;
}

unsigned countBugs(const std::array<std::string,5> &level, size_t x, size_t y, const std::array<std::string,5> &prevLevel, const std::array<std::string,5> &nextLevel)
{
	unsigned result = 0;
	
	if(y>0 && !(y == 3 && x == 2) && level[y-1][x] == '#')
	{
		result++;
	}
	if(x>0 && !(x == 3 && y == 2) && level[y][x-1] == '#')
	{
		result++;
	}
	if(x+1<level[0].size() && !(x == 1 && y ==2) && level[y][x+1] == '#')
	{
		result++;
	}
	if(y+1<level.size() && !(y == 1 && x == 2) && level[y+1][x] == '#')
	{
		result++;
	}
	if(y==0 && prevLevel[1][2] == '#')
	{
		result++;
	}
	if(x==4 && prevLevel[2][3] == '#')
	{
		result++;
	}
	if(y==4 && prevLevel[3][2] == '#')
	{
		result++;
	}
	if(x==0 && prevLevel[2][1] == '#')
	{
		result++;
	}
	if(y == 1 && x == 2)
	{
		if(nextLevel[0][0] == '#')
		{
			result++;
		}
		if(nextLevel[0][1] == '#')
		{
			result++;
		}
		if(nextLevel[0][2] == '#')
		{
			result++;
		}
		if(nextLevel[0][3] == '#')
		{
			result++;
		}
		if(nextLevel[0][4] == '#')
		{
			result++;
		}
	}
	if(y == 2 && x == 3)
	{
		if(nextLevel[0][4] == '#')
		{
			result++;
		}
		if(nextLevel[1][4] == '#')
		{
			result++;
		}
		if(nextLevel[2][4] == '#')
		{
			result++;
		}
		if(nextLevel[3][4] == '#')
		{
			result++;
		}
		if(nextLevel[4][4] == '#')
		{
			result++;
		}
	}
	if(y == 3 && x == 2)
	{
		if(nextLevel[4][0] == '#')
		{
			result++;
		}
		if(nextLevel[4][1] == '#')
		{
			result++;
		}
		if(nextLevel[4][2] == '#')
		{
			result++;
		}
		if(nextLevel[4][3] == '#')
		{
			result++;
		}
		if(nextLevel[4][4] == '#')
		{
			result++;
		}
	}
	if(y == 2 && x == 1)
	{
		if(nextLevel[0][0] == '#')
		{
			result++;
		}
		if(nextLevel[1][0] == '#')
		{
			result++;
		}
		if(nextLevel[2][0] == '#')
		{
			result++;
		}
		if(nextLevel[3][0] == '#')
		{
			result++;
		}
		if(nextLevel[4][0] == '#')
		{
			result++;
		}
	}
	return result;
}

uint64_t rate(const std::array<std::string,5> &map)
{
	uint64_t result = 0;
	
	uint64_t points = 1;
	
	for(unsigned i=0; i<map.size(); i++)
	{
		for(unsigned j=0; j<map[0].size(); j++)
		{
			if(map[i][j] == '#')
			{
				result += points;
			}
			points *= 2;
		}
	}
	return result;
}

uint64_t getRating(std::array<std::string,5> map)
{
	uint64_t result = 0;
	
	std::unordered_map<uint64_t,unsigned> cache;
	
	bool done = false;
	while(!done)
	{
		auto newMap = map;
		
		uint64_t bioRating = rate(newMap);
		if(cache.count(bioRating) == 1)
		{
			result = bioRating;
			break;
		}
		else
		{
			cache[bioRating] = 1;
		}
		
		for(unsigned i=0; i<map.size(); i++)
		{
			for(unsigned j=0; j<map[0].size(); j++)
			{
				unsigned bugs = countBugs(map, j, i);
				if(newMap[i][j] == '#' && bugs != 1)
				{
					newMap[i][j] = '.';
				}
				else if(newMap[i][j] == '.' && (bugs == 1 || bugs == 2))
				{
					newMap[i][j] = '#';
				}
			}
		}
		map = newMap;
	}

	return result;
}

unsigned countBugsInLevel(const std::array<std::string,5> &level)
{
	unsigned result = 0;
	
	for(auto& line:level)
	{
		for(auto& element:line)
		{
			if(element == '#')
			{
				result++;
			}
		}
	}
	
	return result;
}

uint64_t countRecursiveBugs(std::array<std::string,5> &level)
{
	uint64_t result = 0;
	
	std::vector<std::array<std::string,5>> levels;
	
	auto emptyLevel = level;
	for(auto& line:emptyLevel)
	{
		for(auto& element:line)
		{
			element = '.';
		}
	}
	
	levels.push_back(emptyLevel);
	levels.push_back(level);
	levels.push_back(emptyLevel);
	
	for(unsigned count=0; count<200; count++)
	{
		bool addNewTop = false;
		bool addNewBottom = false;
		std::vector<std::array<std::string,5>> newLevels;
		
		for(unsigned index=0; index<levels.size(); index++)
		{
			if(index == 0 && countBugsInLevel(levels[index]) > 0)
			{
				addNewTop = true;
			}
			else if(index+1 == levels.size() && countBugsInLevel(levels[index]) > 0)
			{
				addNewBottom = true;
			}
			
			auto newLevel = levels[index];
			
			for(unsigned i=0; i<level.size(); i++)
			{
				for(unsigned j=0; j<level[0].size(); j++)
				{
					if(!(i==2 && j==2))
					{
						std::array<std::string,5> prevLevel;
						std::array<std::string,5> nextLevel;
						if(index>0)
						{
							prevLevel = levels[index-1];
						}
						else
						{
							prevLevel = emptyLevel;
						}
						if(index+1<levels.size())
						{
							nextLevel = levels[index+1];
						}
						else
						{
							nextLevel = emptyLevel;
						}
						
						unsigned bugs = countBugs(levels[index], j, i, prevLevel, nextLevel);
						if(newLevel[i][j] == '#' && bugs != 1)
						{
							newLevel[i][j] = '.';
						}
						else if(newLevel[i][j] == '.' && (bugs == 1 || bugs == 2))
						{
							newLevel[i][j] = '#';
						}
					}
				}
			}
			newLevels.push_back(newLevel);
		}
		if(addNewTop)
		{
			newLevels.insert(newLevels.begin(),emptyLevel);
		}
		if(addNewBottom)
		{
			newLevels.push_back(emptyLevel);
		}
		levels = newLevels;
	}
	
	for(auto& level:levels)
	{
		result += countBugsInLevel(level);
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::array<std::string,5> map;
   
  parseInput(map);
	
 	resultA = getRating(map);
	resultB = countRecursiveBugs(map);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
 	
  return 0;
}
