#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <array>

void parseInput(std::vector<std::string> &layers)
{
  std::string line;
  
  std::ifstream input("input.txt");
  if(input.is_open())
  {
  	getline(input,line);
  	for(unsigned i=0; i<line.size(); i+=25*6)
  	{
  		layers.push_back(line.substr(i,25*6));
  	}
  }
  input.close();
}

uint64_t findLayer(const std::vector<std::string> &layers)
{
	uint64_t result = 0;
	
	unsigned zeros = 25*6;
	for(auto &layer : layers)
	{
		unsigned newZeros = std::count_if(layer.begin(),layer.end(), [](char c){return c == '0'; });
		
		if(newZeros < zeros)
		{
			zeros = newZeros;
			unsigned ones = std::count_if(layer.begin(),layer.end(), [](char c){return c == '1'; });
			unsigned twos = std::count_if(layer.begin(),layer.end(), [](char c){return c == '2'; });
			result = ones*twos;
		}
	}
	
	return result;
}

void drawImage(const std::vector<std::string> &layers)
{
	std::array<std::string,6> screen;
	
	for(unsigned i=0; i<6; i++)
	{
		screen[i] = layers[0].substr(25*i,25);
	}
	
	for(auto &layer : layers)
	{
		for(unsigned i=0; i<6; i++)
		{
			for(unsigned j=0; j<25; j++)
			{
				if(screen[i][j] == '2')
				{
					screen[i][j] = layer[25*i+j];
				}
				if(screen[i][j] == '0')
				{
					screen[i][j] = ' ';
				}
				else if(screen[i][j] == '1')
				{
					screen[i][j] = '#';
				}
			}
		}
	}
	
	for(unsigned i=0; i<6; i++)
	{
		std::cout << screen[i] << std::endl;
	}
}

int main()
{
	uint64_t resultA = 0;
	std::vector<std::string> layers;
   
  parseInput(layers);
	
 	resultA = findLayer(layers);
 
  std::cout << "resultA: " << resultA << '\n' << '\n';

 	drawImage(layers);
 	
  return 0;
}
