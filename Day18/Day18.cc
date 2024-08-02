#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <array>
#include <algorithm>
#include <utility>

struct Node
{
	unsigned id = 0;
	size_t x = 0;
	size_t y = 0;
	std::vector<std::pair<unsigned,unsigned>> neighbors;
	char key = 0;
	char lock = 0;
	bool start = false;
};

struct Walker
{
	unsigned atNode = 0;
	unsigned prevNode = 0;
	unsigned steps = 0;
	unsigned keyCount = 0;
	std::array<bool, 26> keys{false};
};

struct orderWalkers
{
	bool operator()(const Walker walker1, const Walker walker2) const { if(walker1.keyCount != walker2.keyCount) return walker1.keyCount > walker2.keyCount; else return walker1.steps < walker2.steps; }
};

void parseInput(std::vector<std::string> &map)
{
	std::string line;
	
  std::ifstream input("input.txt");
  if(input.is_open())
  {	
  	while(getline(input, line))
  	{
  		map.push_back(line);
  	}
  }
  input.close();
}

unsigned numberOfPaths(const std::vector<std::string> &map, size_t x, size_t y)
{
	unsigned result = 0;
	
	if(map[y-1][x] != '#')
	{
		result++;
	}
	if(map[y][x+1] != '#')
	{
		result++;
	}
	if(map[y+1][x] != '#')
	{
		result++;
	}
	if(map[y][x-1] != '#')
	{
		result++;
	}
	
	return result;
}
void removeDeadEnd(std::vector<std::string> &map, size_t x, size_t y)
{
	if((map[y][x] == '.' || (map[y][x] >= 'A' && map[y][x] <= 'Z')) && numberOfPaths(map, x, y) == 1)
	{
		map[y][x] = '#';
		
		if((map[y-1][x] == '.' || (map[y-1][x] >= 'A' && map[y-1][x] <= 'Z')))
		{
			removeDeadEnd(map, x, y-1);
		}
		else if((map[y][x+1] == '.' || (map[y][x+1] >= 'A' && map[y][x+1] <= 'Z')))
		{
			removeDeadEnd(map, x+1, y);
		}
		else if((map[y+1][x] == '.' || (map[y+1][x] >= 'A' && map[y+1][x] <= 'Z')))
		{
			removeDeadEnd(map, x, y+1);
		}
		else if((map[y][x-1] == '.' || (map[y][x-1] >= 'A' && map[y][x-1] <= 'Z')))
		{
			removeDeadEnd(map, x-1, y);
		}
	}
}

void removeDeadEnds(std::vector<std::string> &map)
{
	for(unsigned i=1; i<map.size()-1; i++)
	{
		for(unsigned j=1; j<map[0].size()-1; j++)
		{
			if(map[i][j] == '.' && numberOfPaths(map, j, i) == 1)
			{
				removeDeadEnd(map, j, i);
			}
		}
	}
}

void findNeighbors(Node &node, std::vector<Node> &nodes, std::vector<std::string> &map, size_t x, size_t y)
{
	unsigned steps = 1;
	while(map[y][x] == '.')
	{
		map[y][x] = '#';
		if(map[y-1][x] != '#')
		{
			y--;
		}
		else if(map[y][x+1] != '#')
		{
			x++;
		}
		else if(map[y+1][x] != '#')
		{
			y++;
		}
		else if(map[y][x-1] != '#')
		{
			x--;
		}
		steps++;
	}
	unsigned neighborId = 0;
	
	for(auto& neighborNode:nodes)
	{
		if(y == neighborNode.y && x == neighborNode.x)
		{
			neighborId = neighborNode.id;
			break;
		}
	}
	node.neighbors.push_back(std::make_pair(neighborId,steps));
	nodes[neighborId].neighbors.push_back(std::make_pair(node.id,steps));
}

void connectNeighbors(std::vector<Node> &nodes, std::vector<std::string> &map)
{
	for(auto& node:nodes)
	{
		size_t x = node.x;
		size_t y = node.y;
		
		map[y][x] = '#';
		
		if(map[y-1][x] != '#')
		{
			findNeighbors(node, nodes, map, x, y-1);
		}
		if(map[y][x+1] != '#')
		{
			findNeighbors(node, nodes, map, x+1, y);
		}
		if(map[y+1][x] != '#')
		{
			findNeighbors(node, nodes, map, x, y+1);
		}
		if(map[y][x-1] != '#')
		{
			findNeighbors(node, nodes, map, x-1, y);
		}
	}
	
	for(unsigned i=0; i<nodes.size(); i++)
	{
		if(nodes[i].start == false && nodes[i].key == 0 && nodes[i].lock == 0)
		{
			Node& node = nodes[i];
			for(unsigned j=0; j<node.neighbors.size()-1; j++)
			{
				for(unsigned k=j+1; k<node.neighbors.size(); k++)
				{
					unsigned steps = node.neighbors[j].second + node.neighbors[k].second;
					
					bool connected = false;
					for(unsigned ni=0; ni<nodes[node.neighbors[j].first].neighbors.size(); ni++)
					{
						if(nodes[node.neighbors[j].first].neighbors[ni].first == node.id)
						{
							nodes[node.neighbors[j].first].neighbors.erase(nodes[node.neighbors[j].first].neighbors.begin()+ni);
						}
						if(nodes[node.neighbors[j].first].neighbors[ni].first == node.neighbors[k].first)
						{
							connected = true;
							if(nodes[node.neighbors[j].first].neighbors[ni].second > steps)
							{
								nodes[node.neighbors[j].first].neighbors[ni].second = steps;
							}
						}
					}
					if(!connected)
					{
						nodes[node.neighbors[j].first].neighbors.push_back(std::make_pair(node.neighbors[k].first,steps));
					}
					
					connected = false;
					for(unsigned ni=0; ni<nodes[node.neighbors[k].first].neighbors.size(); ni++)
					{
						if(nodes[node.neighbors[k].first].neighbors[ni].first == node.id)
						{
							nodes[node.neighbors[k].first].neighbors.erase(nodes[node.neighbors[k].first].neighbors.begin()+ni);
						}
						if(nodes[node.neighbors[k].first].neighbors[ni].first == node.neighbors[j].first)
						{
							connected = true;
							if(nodes[node.neighbors[k].first].neighbors[ni].second > steps)
							{
								nodes[node.neighbors[k].first].neighbors[ni].second = steps;
							}
						}
					}
					if(!connected)
					{
						nodes[node.neighbors[k].first].neighbors.push_back(std::make_pair(node.neighbors[j].first,steps));
					}
				}	
			}
		}
	}
}

uint64_t getKeys(Walker &walker, std::vector<Node> &nodes, unsigned allKeysCount)
{
	uint64_t result = UINT64_MAX;
	
	std::deque<Walker> openList;
	std::vector<Walker> closedList;
	closedList.reserve(nodes.size()*nodes.size());
	openList.push_back(walker);
	
	while(!openList.empty())
	{
		walker = openList.front();
		openList.pop_front();
		char key = nodes[walker.atNode].key;
		if(key != 0 && walker.keys[key-'a'] == false)
		{
			walker.keys[key-'a'] = true;
			walker.keyCount++;
		}
		if(walker.keyCount == allKeysCount)
		{
			if(result > walker.steps)
			{
				result = walker.steps;
			}
		}
		closedList.push_back(walker);
		
		Node node = nodes[walker.atNode];
		Walker newWalker = walker;
		Walker prevWalker;
		newWalker.prevNode = walker.atNode;
		bool moved = false;
		
		for(auto neighbor:node.neighbors)
		{
			newWalker.atNode = neighbor.first;
			newWalker.steps = walker.steps + neighbor.second;
			if(newWalker.atNode == walker.prevNode)
			{
				prevWalker = newWalker;
			}
			else
			{
				if(newWalker.steps < result)
				{
					if(std::find_if(closedList.begin(), closedList.end(), [newWalker](Walker cWalker){ return newWalker.atNode == cWalker.atNode && newWalker.keys == cWalker.keys && newWalker.steps >= cWalker.steps; }) == std::end(closedList))
					{
						char lock = nodes[newWalker.atNode].lock;
						if(!(lock != 0 && newWalker.keys[lock-'A'] == false))
						{
							moved = true;
							openList.push_back(newWalker);
						}
					}
				}
			}
		}
		if(!moved)
		{
			if(prevWalker.steps < result)
			{
				if(std::find_if(closedList.begin(), closedList.end(), [prevWalker](Walker cWalker){ return prevWalker.atNode == cWalker.atNode && prevWalker.keys == cWalker.keys && prevWalker.steps >= cWalker.steps; }) == std::end(closedList))
				{
					char lock = nodes[prevWalker.atNode].lock;
					if(!(lock != 0 && prevWalker.keys[lock-'A'] == false))
					{
						openList.push_back(prevWalker);
					}
				}
			}
		}
		std::sort(openList.begin(), openList.end(), orderWalkers());

	}
	
	return result;
}

uint64_t findKeys(std::vector<std::string> map)
{
	uint64_t result = 0;
	
	std::vector<Node> nodes;
	unsigned id = 0;
	
	for(unsigned i=1; i<map.size()-1; i++)
	{
		for(unsigned j=1; j<map[0].size()-1; j++)
		{
			if((map[i][j] == '.' && numberOfPaths(map, j, i) >= 3) || (map[i][j] >= '@'))
			{
				Node node;
				node.x = j;
				node.y = i;
				node.id = id;
				id++;
				char symbol = map[i][j];
				if(symbol >= 'A' && symbol <= 'Z')
				{
					node.lock = symbol;
				}
				else if(symbol >= 'a' && symbol <= 'z')
				{
					node.key = symbol;
				}
				else if(symbol == '@')
				{
					node.start = true;
				}
				nodes.push_back(node);
				if(symbol == '.')
				{
					map[i][j] = '+';
				}
			}
		}
	}
	
	connectNeighbors(nodes, map);
	
	Walker walker;
	
	for(unsigned i=0; i<nodes.size(); i++)
	{
		if(nodes[i].start == true)
		{
			walker.atNode = nodes[i].id;
			break;
		}
	}
	
	unsigned allKeysCount = 26;
	result = getKeys(walker,nodes,allKeysCount);
	
	return result;
}

uint64_t useBots(std::vector<std::string> &map)
{
	uint64_t result = 0;
	
	bool changedEntry = false;
	
	for(unsigned i=1; i<map.size()-1; i++)
	{
		for(unsigned j=1; j<map[0].size()-1; j++)
		{
			if(map[i][j] >= 'A' && map[i][j] <= 'Z')
			{
				map[i][j] = '.';
			}
			if(!changedEntry && map[i][j] == '@')
			{
				changedEntry = true;
				map[i-1][j-1] = '@';
				map[i-1][j] = '#';
				map[i-1][j+1] = '@';
				map[i][j-1] = '#';
				map[i][j] = '#';
				map[i][j+1] = '#';
				map[i+1][j-1] = '@';
				map[i+1][j] = '#';
				map[i+1][j+1] = '@';
			}
		}
	}
	
	std::vector<Node> nodes;
	unsigned id = 0;
	
	for(unsigned i=1; i<map.size()-1; i++)
	{
		for(unsigned j=1; j<map[0].size()-1; j++)
		{
			if((map[i][j] == '.' && numberOfPaths(map, j, i) >= 3) || (map[i][j] == '@') || map[i][j] >= 'a')
			{
				Node node;
				node.x = j;
				node.y = i;
				node.id = id;
				id++;
				char symbol = map[i][j];
				if(symbol >= 'a' && symbol <= 'z')
				{
					node.key = symbol;
				}
				else if(symbol == '@')
				{
					node.start = true;
				}
				nodes.push_back(node);
				if(symbol == '.')
				{
					map[i][j] = '+';
				}
			}
		}
	}
	
	connectNeighbors(nodes, map);

	std::array<Walker,4> startWalkers;
	unsigned walkerIndex = 0;
	
	for(unsigned i=0; i<nodes.size(); i++)
	{
		if(nodes[i].start == true)
		{
			startWalkers[walkerIndex].atNode = nodes[i].id;
			walkerIndex++;
			if(walkerIndex == 4)
			{
				break;
			}
		}
	}

	unsigned ULKeys = 6;
	unsigned URKeys = 7;
	unsigned DLKeys = 10;
	unsigned DRKeys = 3;
	
	result += getKeys(startWalkers[0], nodes, ULKeys);
	result += getKeys(startWalkers[1], nodes, URKeys);
	result += getKeys(startWalkers[2], nodes, DLKeys);
	result += getKeys(startWalkers[3], nodes, DRKeys);
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<std::string> map;
   
  parseInput(map);
	removeDeadEnds(map);
	
 	resultA = findKeys(map);
	resultB = useBots(map);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
 	
  return 0;
}
