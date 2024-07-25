#include <iostream>
#include <fstream>
#include <array>
#include <unordered_map> 
#include <numeric>

void parseInput(std::array<std::array<int, 6>,4> &moons)
{
  std::string line;
  
  std::ifstream input("input.txt");
  if(input.is_open())
  {
  	unsigned i = 0;
  	while(getline(input,line))
  	{
  		std::array<int, 6> moon{0};
  		size_t pos = line.find_first_of("-1234567890");
  		size_t endpos = line.find_first_of(",", pos);
  		moon[0] = std::stoi(line.substr(pos, endpos-pos));
  		
  		pos = line.find_first_of("-1234567890", endpos);
  		endpos = line.find_first_of(",", pos);
  		moon[1] = std::stoi(line.substr(pos, endpos-pos));
  		
  		pos = line.find_first_of("-1234567890", endpos);
  		endpos = line.find_first_of(",", pos);
  		moon[2] = std::stoi(line.substr(pos, endpos-pos));
  		
  		moons[i] = moon;
  		i++;
  	}  	
  }
  input.close();
}

uint64_t calculateEnergy(std::array<std::array<int, 6>,4> moons)
{
	uint64_t result = 0;
	
	for(unsigned steps=0; steps<100000; steps++)
	{
		for(unsigned i=0; i<moons.size(); i++)
		{
			for(unsigned j=0; j<moons.size(); j++)
			{
				moons[i][3] += (moons[i][0] < moons[j][0]) ? 1 : (moons[i][0] == moons[j][0] ? 0 : -1);
				moons[i][4] += (moons[i][1] < moons[j][1]) ? 1 : (moons[i][1] == moons[j][1] ? 0 : -1);
				moons[i][5] += (moons[i][2] < moons[j][2]) ? 1 : (moons[i][2] == moons[j][2] ? 0 : -1);
			}
		}
		
		for(unsigned i=0; i<moons.size(); i++)
		{
			moons[i][0] += moons[i][3];
			moons[i][1] += moons[i][4];
			moons[i][2] += moons[i][5];
		}
	}

	for(unsigned i=0; i<moons.size(); i++)
	{
		result += (abs(moons[i][0]) + abs(moons[i][1]) + abs(moons[i][2]))*(abs(moons[i][3]) + abs(moons[i][4]) + abs(moons[i][5]));
	}
	
	return result;
}

uint64_t findSamePosition(std::array<std::array<int, 6>,4> &moons)
{
	uint64_t result = 0;
	
	std::unordered_map<uint64_t,unsigned> cacheX;
	std::unordered_map<uint64_t,unsigned> cacheY;
	std::unordered_map<uint64_t,unsigned> cacheZ;
	uint64_t loopX = 0;
	uint64_t loopY = 0;
	uint64_t loopZ = 0;
	unsigned foundLoops = 0;
	
	for(unsigned steps=0; steps<100000000; steps++)
	{
		for(unsigned i=0; i<moons.size(); i++)
		{
			for(unsigned j=0; j<moons.size(); j++)
			{
				moons[i][3] += (moons[i][0] < moons[j][0]) ? 1 : (moons[i][0] == moons[j][0] ? 0 : -1);
				moons[i][4] += (moons[i][1] < moons[j][1]) ? 1 : (moons[i][1] == moons[j][1] ? 0 : -1);
				moons[i][5] += (moons[i][2] < moons[j][2]) ? 1 : (moons[i][2] == moons[j][2] ? 0 : -1);
			}
		}
		
		for(unsigned i=0; i<moons.size(); i++)
		{
			moons[i][0] += moons[i][3];
			moons[i][1] += moons[i][4];
			moons[i][2] += moons[i][5];
		}
		
		uint64_t cacheNumX = moons[0][0] + moons[1][0]*100 + moons[2][0]*10000 + moons[0][3]*1000000 + moons[1][3]*100000000 + moons[2][3]*10000000000;
		uint64_t cacheNumY = moons[0][1] + moons[1][1]*100 + moons[2][1]*10000 + moons[0][4]*1000000 + moons[1][4]*100000000 + moons[2][4]*10000000000;
		uint64_t cacheNumZ = moons[0][2] + moons[1][2]*100 + moons[2][2]*10000 + moons[0][5]*1000000 + moons[1][5]*100000000 + moons[2][5]*10000000000;
		
		if(loopX == 0)
		{
			if(cacheX[cacheNumX] == 1)
			{
				foundLoops++;
				loopX = steps;
			}
			cacheX[cacheNumX] += 1;
		}
		
		if(loopY == 0)
		{
			if(cacheY[cacheNumY] == 1)
			{
				foundLoops++;
				loopY = steps;
			}
			cacheY[cacheNumY] += 1;
		}

		if(loopZ == 0)
		{
			if(cacheZ[cacheNumZ] == 1)
			{
				foundLoops++;
				loopZ = steps;
			}
			cacheZ[cacheNumZ] += 1;
		}
		
		if(foundLoops == 3)
		{
			break;
		}
	}

	result = std::lcm(loopX,std::lcm(loopY,loopZ));
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::array<std::array<int, 6>,4> moons;
   
  parseInput(moons);
	
 	resultA = calculateEnergy(moons);
	resultB = findSamePosition(moons);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
 	
  return 0;
}
