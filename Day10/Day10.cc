#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <numeric>
#include <utility>
#include <algorithm>

struct Asteroid
{
	size_t x = 0;
	size_t y = 0;
	float angle = 0;
};

struct quadOddOrdering
{
	bool operator()(const Asteroid* asteroid1, const Asteroid* asteroid2) const { return asteroid1->angle > asteroid2->angle; }
};

struct quadEvenOrdering
{
	bool operator()(const Asteroid* asteroid1, const Asteroid* asteroid2) const { return asteroid1->angle < asteroid2->angle; }
};

void parseInput(std::vector<std::string> &map)
{
  std::string line;
  
  std::ifstream input("input.txt");
  if(input.is_open())
  {
  	while(getline(input,line))
  	{
  		map.push_back(line);
  	}  	
  }
  input.close();
}

std::pair<uint64_t,uint64_t> findAndDestroyAsteroid(const std::vector<std::string> &map)
{
	std::pair<uint64_t,uint64_t> result;
	
	size_t monitorX = 0;
	size_t monitorY = 0;
	
	std::unordered_map<float,Asteroid> asteroids; 
	
	for(unsigned i=0; i<map.size(); i++)
	{
		for(unsigned j=0; j<map[0].size(); j++)
		{
			if(map[i][j] == '#')
			{
				std::unordered_map<float,Asteroid> cache;

				for(unsigned y=0; y<map.size(); y++)
				{
					for(unsigned x=0; x<map[0].size(); x++)
					{
						if(!(y == i && x == j) && map[y][x] == '#')
						{
							float diffy = abs(y-i);
							float diffx = abs(x-j);
							unsigned divisor = std::gcd(abs(y-i),abs(x-j));
							diffy /= divisor;
							diffx /= divisor;
							float cacheNum = 0;
							float angle = 0;
							
							if(diffx == 0)
							{
								angle = 99;
							}
							else
							{
								angle = diffy/diffx;
							}
							cacheNum = angle;
							if(y>=i && x>j)
							{
								cacheNum += 10000;
							}
							else if(y>i && x<=j)
							{
								cacheNum += 20000;
							}
							else if(y<=i && x<j)
							{
								cacheNum += 30000;
							}
							if(cache.count(cacheNum) == 0)
							{
								Asteroid asteroid;
								asteroid.x = abs(x-j);
								asteroid.y = abs(y-i);
								asteroid.angle = angle;
								cache[cacheNum] = asteroid;
							}
						}
					}
				}
				
				if(result.first < cache.size())
				{
					monitorX = j;
					monitorY = i;
					result.first = cache.size();
					asteroids = cache;
				}
			}
		}	
	}
	
	unsigned toDestroy = 200;
	unsigned target = result.first - toDestroy + 1;

	std::vector<Asteroid*> asteroidsInQuadrant;
	
	unsigned asteroidsInQuadrantCount = 0;
	unsigned quadrant = 0;
	
	for(auto &asteroid : asteroids)
	{
		if(asteroid.first >= 30000)
		{
			asteroidsInQuadrantCount++;
		}
	}
	if(asteroidsInQuadrantCount >= target)
	{
		quadrant = 3;
		for(auto &asteroid : asteroids)
		{
			if(asteroid.first >= 30000)
			{
				asteroidsInQuadrant.push_back(&asteroid.second);
			}
		}
		std::sort(asteroidsInQuadrant.begin(), asteroidsInQuadrant.end(), quadOddOrdering());
	}
	else
	{
		target -= asteroidsInQuadrantCount;
		asteroidsInQuadrantCount = 0;
	}
	if(asteroidsInQuadrantCount == 0)
	{
		for(auto &asteroid : asteroids)
		{
			if(asteroid.first >= 20000 && asteroid.first < 30000)
			{
				asteroidsInQuadrantCount++;
			}
		}
		if(asteroidsInQuadrantCount >= target)
		{
			quadrant = 2;
			for(auto &asteroid : asteroids)
			{
				if(asteroid.first >= 20000 && asteroid.first < 30000)
				{
					asteroidsInQuadrant.push_back(&asteroid.second);
				}
			}
			std::sort(asteroidsInQuadrant.begin(), asteroidsInQuadrant.end(), quadEvenOrdering());
		}
		else
		{
			target -= asteroidsInQuadrantCount;
			asteroidsInQuadrantCount = 0;
		}
	}
	if(asteroidsInQuadrantCount == 0)
	{
		for(auto &asteroid : asteroids)
		{
			if(asteroid.first >= 10000 && asteroid.first < 20000)
			{
				asteroidsInQuadrantCount++;
			}
		}
		if(asteroidsInQuadrantCount >= target)
		{
			quadrant = 1;
			for(auto &asteroid : asteroids)
			{
				if(asteroid.first >= 10000 && asteroid.first < 20000)
				{
					asteroidsInQuadrant.push_back(&asteroid.second);
				}
			}
			std::sort(asteroidsInQuadrant.begin(), asteroidsInQuadrant.end(), quadOddOrdering());
		}
		else
		{
			target -= asteroidsInQuadrantCount;
			asteroidsInQuadrantCount = 0;
		}
	}
	if(asteroidsInQuadrantCount == 0)
	{
		for(auto &asteroid : asteroids)
		{
			if(asteroid.first < 10000)
			{
				asteroidsInQuadrant.push_back(&asteroid.second);
			}
		}
		std::sort(asteroidsInQuadrant.begin(), asteroidsInQuadrant.end(), quadEvenOrdering());
	}
	
	
	Asteroid* targetAsteroid = asteroidsInQuadrant[target-1];

	int targetY = targetAsteroid->y;
	int targetX = targetAsteroid->x;
	
	switch(quadrant)
	{
		case 0:
		{
			targetY = -targetY;
			break;
		}
		case 2:
		{
			targetX = -targetX;
			break;
		}
		case 3:
		{
			targetY = -targetY;
			targetX = -targetX;
			break;
		}
	}	
	
	do
	{
		monitorX += targetX;
		monitorY += targetY;
	}
	while(map[monitorY][monitorX] != '#');
	
	result.second = (100*monitorX) + monitorY;
	
	return result;
}

int main()
{
	std::pair<uint64_t, uint64_t> result;
	
	std::vector<std::string> map;
   
  parseInput(map);
	
 	result = findAndDestroyAsteroid(map);
 
  std::cout << "resultA: " << result.first << '\n';
  std::cout << "resultB: " << result.second << std::endl;
 	
  return 0;
}
