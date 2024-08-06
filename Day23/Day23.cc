#include "../IntcodeComputer/IntcodeComputer.h"
#include <fstream>
#include <iostream>
#include <array>
#include <vector>
#include <deque>

struct Packet
{
	unsigned adress = 0;
	int64_t x = 0;
	int64_t y = 0;
};

void parseInput(std::vector<int64_t> &program)
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
			program.push_back(std::stoll(line.substr(pos,endpos-pos)));
  		pos = line.find_first_of("-1234567890",endpos);
		}
  }
  input.close();
}

uint64_t startNetwork(std::vector<int64_t> baseProgram)
{
	uint64_t result = 0;
	
	std::array<IntcodeComputer,50> computers;
	std::array<std::deque<Packet>,50> messageQueues;
	bool done = false;
	
	for(unsigned i=0; i<50; i++)
	{
		IntcodeComputer comp;
		auto program = baseProgram;
		comp.loadProgram(program);
		comp.pushValue(i);
		comp.executeProgram();
		while(!comp.outbox.empty())
		{
			Packet packet;
			packet.adress = comp.fetchValue();
			packet.x = comp.fetchValue();
			packet.y = comp.fetchValue();
			if(packet.adress == 255)
			{
				result = packet.y;
				break;
			}
			messageQueues[packet.adress].push_back(packet);
		}	
		if(done)
		{
			break;
		}
		computers[i] = comp;
	}


	while(!done)
	{
		for(unsigned i=0; i<50; i++)
		{
			if(!messageQueues[i].empty())
			{
				Packet packet = messageQueues[i].front();
				messageQueues[i].pop_front();
				computers[i].pushValue(packet.x);
				computers[i].pushValue(packet.y);
			}
			else
			{
				computers[i].pushValue(-1);
			}
			computers[i].executeProgram();
			while(!computers[i].outbox.empty())
			{
				Packet packet;
				packet.adress = computers[i].fetchValue();
				packet.x = computers[i].fetchValue();
				packet.y = computers[i].fetchValue();
				if(packet.adress == 255)
				{
					done = true;
					result = packet.y;
					break;
				}
				messageQueues[packet.adress].push_back(packet);
			}
			if(done)
			{
				break;
			}	
		}
	}
	
	return result;
}

int64_t runWithNAT(std::vector<int64_t> &baseProgram)
{
	int64_t result = INT64_MAX;
	
	std::array<IntcodeComputer,50> computers;
	std::array<std::deque<Packet>,50> messageQueues;
	bool done = false;
	
	for(unsigned i=0; i<50; i++)
	{
		IntcodeComputer comp;
		auto program = baseProgram;
		comp.loadProgram(program);
		comp.pushValue(i);
		comp.executeProgram();
		while(!comp.outbox.empty())
		{
			Packet packet;
			packet.adress = comp.fetchValue();
			packet.x = comp.fetchValue();
			packet.y = comp.fetchValue();
			messageQueues[packet.adress].push_back(packet);
		}	
		computers[i] = comp;
	}

	Packet NATPacket;
	
	while(!done)
	{
		bool idle = true;
		for(auto queue:messageQueues)
		{
			if(!queue.empty())
			{
				idle = false;
				break;
			}
		}
		if(idle)
		{
			if(result == NATPacket.y)
			{
				done = true;
				break;
			}
			messageQueues[0].push_back(NATPacket);
			result = NATPacket.y;
		}
		for(unsigned i=0; i<50; i++)
		{
			if(!messageQueues[i].empty())
			{
				Packet packet = messageQueues[i].front();
				messageQueues[i].pop_front();
				computers[i].pushValue(packet.x);
				computers[i].pushValue(packet.y);
			}
			else
			{
				computers[i].pushValue(-1);
			}
			computers[i].executeProgram();
			while(!computers[i].outbox.empty())
			{
				Packet packet;
				packet.adress = computers[i].fetchValue();
				packet.x = computers[i].fetchValue();
				packet.y = computers[i].fetchValue();
				if(packet.adress == 255)
				{
					NATPacket = packet;
				}
				else
				{
					messageQueues[packet.adress].push_back(packet);
				}
			}
		}
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	int64_t resultB = 0;
	
	std::vector<int64_t> program;
	
  parseInput(program);
	
	resultA = startNetwork(program);
	
	resultB = runWithNAT(program);

	std::cout << "ResultA: " << resultA << '\n';
	std::cout << "ResultB: " << resultB << '\n';
	
  return 0;
}
