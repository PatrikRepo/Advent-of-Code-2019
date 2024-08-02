#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <tuple>
#include <algorithm>

struct Porter
{
	std::string name;
	size_t outerX = 0;
	size_t outerY = 0;
	size_t innerX = 0;
	size_t innerY = 0;
};

struct Node
{
	unsigned id = 0;
	size_t x = 0;
	size_t y = 0;
	std::vector<std::tuple<unsigned,unsigned,int>> neighbors;
};

struct Walker
{
	unsigned atNode = 0;
	unsigned steps = 0;
	int level = 0;
	unsigned levelShifts = 0;
};

struct orderWalkers
{
	bool operator()(const Walker walker1, const Walker walker2) const { return walker1.steps < walker2.steps; }
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
	if(map[y][x] == '.' && numberOfPaths(map, x, y) == 1)
	{
		map[y][x] = '#';
		
		if(map[y-1][x] == '.')
		{
			removeDeadEnd(map, x, y-1);
		}
		else if(map[y][x+1] == '.')
		{
			removeDeadEnd(map, x+1, y);
		}
		else if(map[y+1][x] == '.')
		{
			removeDeadEnd(map, x, y+1);
		}
		else if(map[y][x-1] == '.')
		{
			removeDeadEnd(map, x-1, y);
		}
	}
}

void removeDeadEnds(std::vector<std::string> &map)
{
	for(unsigned i=3; i<map.size()-3; i++)
	{
		for(unsigned j=3; j<map[0].size()-3; j++)
		{
			if(map[i][j] == '.' && numberOfPaths(map, j, i) == 1)
			{
				removeDeadEnd(map, j, i);
			}
		}
	}
}

void findPorters(std::vector<Porter> &porters, const std::vector<std::string> &map)
{
	for(unsigned i=3; i<map[0].size()-3; i++)
	{
		if(map[0][i] != ' ')
		{
			Porter porter;
			porter.name += map[0][i];
			porter.name += map[1][i];
			porter.outerX = i;
			porter.outerY = 2;
			porters.push_back(porter);
		}
	}
	for(unsigned i=3; i<map[0].size()-3; i++)
	{
		if(map[map.size()-2][i] != ' ')
		{
			Porter porter;
			porter.name += map[map.size()-2][i];
			porter.name += map[map.size()-1][i];
			porter.outerX = i;
			porter.outerY = map.size()-3;
			porters.push_back(porter);
		}
	}
	for(unsigned i=3; i<map.size()-3; i++)
	{
		if(map[i][0] != ' ')
		{
			Porter porter;
			porter.name += map[i][0]; 
			porter.name += map[i][1];
			porter.outerX = 2;
			porter.outerY = i;
			porters.push_back(porter);
		}
	}
	for(unsigned i=3; i<map.size()-3; i++)
	{
		if(map[i][map[0].size()-2] != ' ')
		{
			Porter porter;
			porter.name += map[i][map[0].size()-2];
			porter.name += map[i][map[0].size()-1];
			porter.outerX = map[0].size()-3;
			porter.outerY = i;
			porters.push_back(porter);
		}
	}
	
	size_t y = map.size()/2;
	size_t posLeftX = map[y].find_last_not_of(" ",map[0].size()/2);
	if(map[y][posLeftX] != '.' && map[y][posLeftX] != '#')
	{
		posLeftX--;
	}
	else
	{
		posLeftX++;
	}
	
	size_t posRightX = map[y].find_first_not_of(" ",posLeftX+2);
	if(map[y][posRightX] != '.' && map[y][posRightX] != '#')
	{
		posRightX++;
	}
	else
	{
		posRightX--;
	}
	
	while(map[y-1][posLeftX] != '.' && map[y-1][posLeftX] != '#')
	{
		y--;
	}
	size_t posUpY = y;
	
	while(map[y+1][posLeftX] != '.' && map[y+1][posLeftX] != '#')
	{
		y++;
	}
	size_t posDownY = y;
	
	for(unsigned i=posLeftX; i<=posRightX; i++)
	{
		if(map[posUpY][i] != ' ')
		{
			std::string name;
			name += map[posUpY][i]; 
			name += map[posUpY+1][i];
			for(auto& porter:porters)
			{
				if(porter.name == name)
				{
					porter.innerX = i;
					porter.innerY = posUpY-1;
				}
			}
		}
	}
	for(unsigned i=posLeftX; i<=posRightX; i++)
	{
		if(map[posDownY][i] != ' ')
		{
			std::string name;
			name += map[posDownY-1][i];
			name += map[posDownY][i];
			for(auto& porter:porters)
			{
				if(porter.name == name)
				{
					porter.innerX = i;
					porter.innerY = posDownY+1;
				}
			}
		}
	}
	for(unsigned i=posUpY; i<=posDownY; i++)
	{
		if(map[i][posLeftX] != ' ')
		{
			std::string name;
			name += map[i][posLeftX];
			name += map[i][posLeftX+1];
			for(auto& porter:porters)
			{
				if(porter.name == name)
				{
					porter.innerX = posLeftX-1;
					porter.innerY = i;
				}
			}
		}
	}
	for(unsigned i=posUpY; i<=posDownY; i++)
	{
		if(map[i][posRightX] != ' ')
		{
			std::string name;
			name += map[i][posRightX-1];
			name += map[i][posRightX];
			for(auto& porter:porters)
			{
				if(porter.name == name)
				{
					porter.innerX = posRightX+1;
					porter.innerY = i;
				}
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
		if(map[y-1][x] == '.' || map[y-1][x] == '+')
		{
			y--;
		}
		else if(map[y][x+1] == '.' || map[y][x+1] == '+')
		{
			x++;
		}
		else if(map[y+1][x] == '.' || map[y+1][x] == '+')
		{
			y++;
		}
		else if(map[y][x-1] == '.' || map[y][x-1] == '+')
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
	node.neighbors.push_back(std::make_tuple(neighborId,steps,0));
	nodes[neighborId].neighbors.push_back(std::make_tuple(node.id,steps,0));
}

void connectThroughPorters(std::vector<Node> &nodes, const std::vector<Porter> &porters)
{
	for(auto& node:nodes)
	{
		for(auto& porter:porters)
		{
			if((node.x == porter.outerX && node.y == porter.outerY) || (node.x == porter.innerX && node.y == porter.innerY))
			{
				for(auto& portNode:nodes)
				{
					if((node.id != portNode.id) && ((portNode.x == porter.outerX && portNode.y == porter.outerY) || (portNode.x == porter.innerX && portNode.y == porter.innerY)))
					{
						int shift = 0;
						if(node.x == porter.innerX)
						{
							shift = 1;
						}
						else
						{
							shift = -1;
						}
						node.neighbors.push_back(std::make_tuple(portNode.id,1,shift));
						break;
					}
				}
				break;
			}
		}
	}
}

void connectNeighbors(std::vector<Node> &nodes, const std::vector<Porter> &porters, std::vector<std::string> &map)
{
	for(auto& node:nodes)
	{
		size_t x = node.x;
		size_t y = node.y;
		
		map[y][x] = '#';
		
		if(map[y-1][x] == '.' || map[y-1][x] == '+')
		{
			findNeighbors(node, nodes, map, x, y-1);
		}
		if(map[y][x+1] == '.' || map[y][x+1] == '+')
		{
			findNeighbors(node, nodes, map, x+1, y);
		}
		if(map[y+1][x] == '.' || map[y+1][x] == '+')
		{
			findNeighbors(node, nodes, map, x, y+1);
		}
		if(map[y][x-1] == '.' || map[y][x-1] == '+')
		{
			findNeighbors(node, nodes, map, x-1, y);
		}
	}
	
	connectThroughPorters(nodes, porters);
}


uint64_t shortestPath(std::vector<std::string> map)
{
	uint64_t result = 0;
	
	std::vector<Porter> porters;
	std::vector<Node> nodes;
	
	findPorters(porters, map);
	
	unsigned id = 0;
	for(unsigned i=2; i<map.size()-2; i++)
	{
		for(unsigned j=2; j<map[0].size()-2; j++)
		{
			if(map[i][j] == '.' && ((numberOfPaths(map, j, i) >= 3) || (map[i-1][j] >= 'A') || (map[i+1][j] >= 'A') || (map[i][j-1] >= 'A') || (map[i][j+1] >= 'A')))
			{
				Node node;
				node.x = j;
				node.y = i;
				node.id = id;
				id++;
				nodes.push_back(node);
				map[i][j] = '+';
			}
		}
	}
	
	connectNeighbors(nodes, porters, map);
	
	size_t startX = 0;
	size_t startY = 0;
	size_t endX = 0;
	size_t endY = 0;
	bool setOne = false;
	
	for(auto& porter:porters)
	{
		if(porter.name == "AA")
		{
			startX = porter.outerX;
			startY = porter.outerY;
			if(!setOne)
			{
				setOne = true;
			}
			else
			{
				break;
			}
		}
		else if(porter.name == "ZZ")
		{
			endX = porter.outerX;
			endY = porter.outerY;
			if(!setOne)
			{
				setOne = true;
			}
			else
			{
				break;
			}
		}
	}
	
	unsigned startNode = 0;
	unsigned endNode = 0;
	
	setOne = false;
	for(auto& node:nodes)
	{
		if(startX == node.x && startY == node.y)
		{
			startNode = node.id;
			if(!setOne)
			{
				setOne = true;
			}
			else
			{
				break;
			}
		}
		else if(endX == node.x && endY == node.y)
		{
			endNode = node.id;
			if(!setOne)
			{
				setOne = true;
			}
			else
			{
				break;
			}
		}
	}
	
	Walker walker;
	walker.atNode = startNode;
	
	std::deque<Walker> openList;
	std::deque<Walker> closedList;
	openList.push_back(walker);
	
	while(!openList.empty())
	{
		walker = openList.front();
		openList.pop_front();
		if(walker.atNode == endNode)
		{
			result = walker.steps;
			break;
		}
		closedList.push_back(walker);
		
		Walker newWalker;
		for(auto& neighbor:nodes[walker.atNode].neighbors)
		{
			newWalker.atNode = std::get<0>(neighbor);
			newWalker.steps = walker.steps + std::get<1>(neighbor);
			if(std::find_if(closedList.begin(),closedList.end(),[newWalker](Walker cWalker) { return newWalker.atNode == cWalker.atNode && newWalker.steps >= cWalker.steps; }) == std::end(closedList))
			{
				openList.push_back(newWalker);
			}
		}
		std::sort(openList.begin(),openList.end(),orderWalkers());
	}
	
	return result;
}

uint64_t recursivePath(std::vector<std::string> &map)
{
	uint64_t result = UINT64_MAX;
	
	std::vector<Porter> porters;
	std::vector<Node> nodes;
	
	findPorters(porters, map);
	
	unsigned id = 0;
	for(unsigned i=2; i<map.size()-2; i++)
	{
		for(unsigned j=2; j<map[0].size()-2; j++)
		{
			if(map[i][j] == '.' && ((numberOfPaths(map, j, i) >= 3) || (map[i-1][j] >= 'A') || (map[i+1][j] >= 'A') || (map[i][j-1] >= 'A') || (map[i][j+1] >= 'A')))
			{
				Node node;
				node.x = j;
				node.y = i;
				node.id = id;
				id++;
				nodes.push_back(node);
				map[i][j] = '+';
			}
		}
	}
	
	connectNeighbors(nodes, porters, map);
	
	size_t startX = 0;
	size_t startY = 0;
	size_t endX = 0;
	size_t endY = 0;
	bool setOne = false;
	
	for(auto& porter:porters)
	{
		if(porter.name == "AA")
		{
			startX = porter.outerX;
			startY = porter.outerY;
			if(!setOne)
			{
				setOne = true;
			}
			else
			{
				break;
			}
		}
		else if(porter.name == "ZZ")
		{
			endX = porter.outerX;
			endY = porter.outerY;
			if(!setOne)
			{
				setOne = true;
			}
			else
			{
				break;
			}
		}
	}
	
	unsigned startNode = 0;
	unsigned endNode = 0;
	
	setOne = false;
	for(auto& node:nodes)
	{
		if(startX == node.x && startY == node.y)
		{
			startNode = node.id;
			if(!setOne)
			{
				setOne = true;
			}
			else
			{
				break;
			}
		}
		else if(endX == node.x && endY == node.y)
		{
			endNode = node.id;
			if(!setOne)
			{
				setOne = true;
			}
			else
			{
				break;
			}
		}
	}

	Walker walker;
	walker.atNode = startNode;
	
	std::deque<Walker> openList;
	std::deque<Walker> closedList;
	openList.push_back(walker);
	
	while(!openList.empty())
	{
		walker = openList.front();
		openList.pop_front();
		if(walker.atNode == endNode)
		{
			if(result > walker.steps)
			{
				result = walker.steps;
				break;
			}
		}
		closedList.push_back(walker);
		
		Walker newWalker;
		for(auto& neighbor:nodes[walker.atNode].neighbors)
		{
			newWalker.atNode = std::get<0>(neighbor);
			newWalker.steps = walker.steps + std::get<1>(neighbor);
			newWalker.level = walker.level + std::get<2>(neighbor);
			newWalker.levelShifts = walker.levelShifts + abs(std::get<2>(neighbor));
			if(std::find_if(closedList.begin(),closedList.end(),[newWalker](Walker cWalker) { return newWalker.atNode == cWalker.atNode && newWalker.steps >= cWalker.steps && newWalker.level == cWalker.level; }) == std::end(closedList))
			{
				if(newWalker.level >= 0 && !(newWalker.level > 0 && (newWalker.atNode == startNode || newWalker.atNode == endNode)))
				{
					openList.push_back(newWalker);
				}
			}
		}
		std::sort(openList.begin(),openList.end(),orderWalkers());
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<std::string> map;
   
  parseInput(map);
	removeDeadEnds(map);
	
 	resultA = shortestPath(map);
	resultB = recursivePath(map);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;
 	
  return 0;
}
