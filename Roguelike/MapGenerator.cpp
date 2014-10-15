#include "MapGenerator.h"
#include "Map.h"
#include <cassert>
#include <ctime> //time
#include <cstdlib> //rand, srand
#include <iostream>


//////////
//  m_width & height are the number of tiles
//  Set seed to -1 for a random map.
//////////
MapGenerator::MapGenerator(int maxFeatures, int chanceRoom, int chanceHall, int seed)
	: m_maxFeatures(maxFeatures), m_chanceRoom(chanceRoom), m_chanceHall(chanceHall)
{
	if (seed == -1)
		seed = static_cast<int>(time(0));

	SetSeed(seed);
}

MapGenerator::~MapGenerator()
{

}

int MapGenerator::GetSeed()
{
	return m_seed;
}
void MapGenerator::SetSeed(int seed)
{
	m_seed = seed;
	srand(seed);
}

void MapGenerator::Generate( Map& map )
{
	//Clears out old map contents.
	map.FillRect(0,0,map.m_width-1, map.m_height-1, Map::Tile::Unused);

	//Dig out a single room in the centre of the map
	MakeRoom(map, map.m_width / 2, map.m_height / 2, 8, 8, GetRandomDirection());

	
		for (auto features = 1; features < m_maxFeatures; ++features)
		{
			if (!MakeFeature(map))
			{
				std::cout << "Unable to place more features (placed " << features << ")." << std::endl;
				break;
			}
		}
 
		if (!MakeStairs(map, Map::Tile::UpStairs))
			std::cout << "Unable to place up stairs." << std::endl;
 
		if (!MakeStairs(map, Map::Tile::DownStairs))
			std::cout << "Unable to place down stairs." << std::endl;
}

bool MapGenerator::MakeRoom(Map& map, int x, int y, int maxWidth, int maxHeight, Direction direction)
	{
		// Minimum room size of 4x4 tiles (2x2 for walking on, the rest is walls)
		auto w = GetRandomInt(4, maxWidth);
		auto h = GetRandomInt(4, maxHeight);
 
		auto left = x;
		auto top = y;
 
		auto right = x;
		auto bottom = y;
 
		if (direction == Direction::North)
		{
			top = y - h;
			left = x - w / 2;
			right = x + (w + 1) / 2;
		}
		else if (direction == Direction::East)
		{
			top = y - h / 2;
			bottom = y + (h + 1) / 2;
			right = x + w;
		}
		else if (direction == Direction::South)
		{
			bottom = y + h;
			left = x - w / 2;
			right = x + (w + 1) / 2;
		}
		else if (direction == Direction::West)
		{
			top = y - h / 2;
			bottom = y + (h + 1) / 2;
			left = x - w;
		}
 
		if (!map.IsInBoundsX(left) || !map.IsInBoundsX(right) || !map.IsInBoundsY(top) || !map.IsInBoundsY(bottom))
			return false;
 
		if (!map.IsAreaUnused(left, top, right, bottom))
			return false;
 
		map.FillRect(left, top, right, bottom, Map::Tile::Wall);
		map.FillRect(left + 1, top + 1, right - 1, bottom - 1, Map::Tile::Floor);
 
		//std::cout << "Room: ( " << left << ", " << top << " ) to ( " << right << ", " << bottom << " )" << std::endl;
 
		return true;
	}

	bool MapGenerator::MakeFeature(Map& map) 
	{
		auto tries = 0;
		auto maxTries = 1000;
 
		for( ; tries != maxTries; ++tries)
		{
			// Pick a random wall or corridor tile.
			// Make sure it has no adjacent doors (looks weird to have doors next to each other).
			// Find a direction from which it's reachable.
			// Attempt to make a feature (room or corridor) starting at this point.
 
			int x = GetRandomInt(1, map.m_width - 2);
			int y = GetRandomInt(1, map.m_height - 2);
 
			if (map.GetTileTypeAt(x, y) != Map::Tile::Wall && map.GetTileTypeAt(x, y) != Map::Tile::Corridor)
				continue;
 
			if (map.IsAdjacentToTileType(x, y, Map::Tile::Door))
				continue;
 
			if (map.GetTileTypeAt(x, y+1) == Map::Tile::Floor || map.GetTileTypeAt(x, y+1) == Map::Tile::Corridor)
			{
				if (MakeFeature(map, x, y, 0, -1, Direction::North))
					return true;
			}
			else if (map.GetTileTypeAt(x-1, y) == Map::Tile::Floor || map.GetTileTypeAt(x-1, y) == Map::Tile::Corridor)
			{
				if (MakeFeature(map, x, y, 1, 0, Direction::East))
					return true;
			}
			else if (map.GetTileTypeAt(x, y-1) == Map::Tile::Floor || map.GetTileTypeAt(x, y-1) == Map::Tile::Corridor)
			{
				if (MakeFeature(map, x, y, 0, 1, Direction::South))
					return true;
			}
			else if (map.GetTileTypeAt(x+1, y) == Map::Tile::Floor || map.GetTileTypeAt(x+1, y) == Map::Tile::Corridor)
			{
				if (MakeFeature(map, x, y, -1, 0, Direction::West))
					return true;
			}
		}
 
		return false;
	}

	bool MapGenerator::MakeFeature(Map& map, int x, int y, int xmod, int ymod, Direction direction)
	{
		// Choose what to build
		auto chance = GetRandomInt(0, 100);
 
		if (chance <= m_chanceRoom)
		{
			if (MakeRoom(map, x + xmod, y + ymod, 8, 6, direction))
			{
				map.SetTile(x, y, Map::Tile::Door);
 
				// Remove wall next to the door.
				map.SetTile(x + xmod, y + ymod, Map::Tile::Floor);
 
				return true;
			}
 
			return false;
		}
		else
		{
			if (MakeCorridor(map, x + xmod, y + ymod, 6, direction))
			{
				map.SetTile(x, y, Map::Tile::Door);
 
				return true;
			}
 
			return false;
		}
	}

	bool MapGenerator::MakeCorridor(Map& map, int x, int y, int maxLength, Direction direction) 
	{
		assert(x >= 0 && x < map.m_width);
		assert(y >= 0 && y < map.m_height);
 
		assert(maxLength > 0 && maxLength <= std::max(map.m_width, map.m_height));
 
		auto length = GetRandomInt(2, maxLength);
 
		auto left = x;
		auto top = y;
 
		auto right = x;
		auto bottom = y;
 
		if (direction == Direction::North)
			top = y - length;
		else if (direction == Direction::East)
			right = x + length;
		else if (direction == Direction::South)
			bottom = y + length;
		else if (direction == Direction::West)
			left = x - length;
 
		if (!map.IsInBoundsX(left) || !map.IsInBoundsX(right) || !map.IsInBoundsY(top) || !map.IsInBoundsY(bottom))
			return false;
 
		if (!map.IsAreaUnused(left, top, right, bottom))
			return false;
 
		map.FillRect(left, top, right, bottom, Map::Tile::Corridor);
 
		//std::cout << "Corridor: ( " << left << ", " << top << " ) to ( " << right << ", " << bottom << " )" << std::endl;
 
		return true;
	}

bool MapGenerator::MakeMap(Map & map)
{
	//Dig out a single room in the centre of the map
	MakeRoom(map, map.m_width / 2, map.m_height / 2, 8, 8, GetRandomDirection());

	
			for (auto features = 1; features < m_maxFeatures; ++features)
		{
			if (!MakeFeature(map))
			{
				std::cout << "Unable to place more features (placed " << features << ")." << std::endl;
				break;
			}
		}
 
		if (!MakeStairs(map, Map::Tile::UpStairs))
			std::cout << "Unable to place up stairs." << std::endl;
 
		if (!MakeStairs(map, Map::Tile::DownStairs))
			std::cout << "Unable to place down stairs." << std::endl;
	
		return true;
}

	bool MapGenerator::MakeStairs(Map& map, Map::Tile tile)
	{
		auto tries = 0;
		auto maxTries = 10000;
 
		for ( ; tries != maxTries; ++tries)
		{
			int x = GetRandomInt( 1, map.m_width - 2);
			int y = GetRandomInt( 1, map.m_height - 2);
 
			if (!map.IsAdjacentToTileType(x, y, Map::Tile::Floor) && !map.IsAdjacentToTileType (x, y, Map::Tile::Corridor))
				continue;
 
			if (map.IsAdjacentToTileType(x, y, Map::Tile::Door))
				continue;
 
			map.SetTile(x, y, tile);
 
			return true;
		}
 
		return false;
	}

	MapGenerator::Direction MapGenerator::GetRandomDirection()
	{
		return Direction(rand() % 4);
	}

	int MapGenerator::GetRandomInt(int min, int max)
	{
		return (rand() % (max - min)) + min;
	}