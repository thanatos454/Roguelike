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
MapGenerator::MapGenerator(int width, int height
						   , int maxFeatures, int chanceRoom, int chanceHall, int seed)
{
	this->m_width = width;
	this->m_height = height;
	this->m_maxFeatures = maxFeatures;
	this->m_chanceRoom = chanceRoom;
	this->m_chanceHall = chanceHall;



	if (seed == -1)
		seed = static_cast<int>(time(0));

	this->SetSeed(seed);


}

MapGenerator::~MapGenerator()
{

}

int MapGenerator::GetSeed()
{
	return this->m_seed;
}
void MapGenerator::SetSeed(int seed)
{
	this->m_seed = seed;
	srand(seed);
}

//TODO: Change GenerateMap to "Generate( Map& m )" and don't save map width/height in mapgenerator, use the width/height contained by Map
Map * MapGenerator::generateMap()
{
	assert(m_maxFeatures > 0);
	assert(m_width > 3 );
	assert(m_height > 3);

	Map * map = new Map(m_width, m_height);

	MakeMap(*map);
	return map;
}

bool MapGenerator::MakeRoom(Map& map, int x, int y, int xMaxLength, int yMaxLength, Direction direction)
	{
		// Minimum room size of 4x4 tiles (2x2 for walking on, the rest is walls)
		auto xLength = GetRandomInt(4, xMaxLength);
		auto yLength = GetRandomInt(4, yMaxLength);
 
		auto xStart = x;
		auto yStart = y;
 
		auto xEnd = x;
		auto yEnd = y;
 
		if (direction == Direction::North)
		{
			yStart = y - yLength;
			xStart = x - xLength / 2;
			xEnd = x + (xLength + 1) / 2;
		}
		else if (direction == Direction::East)
		{
			yStart = y - yLength / 2;
			yEnd = y + (yLength + 1) / 2;
			xEnd = x + xLength;
		}
		else if (direction == Direction::South)
		{
			yEnd = y + yLength;
			xStart = x - xLength / 2;
			xEnd = x + (xLength + 1) / 2;
		}
		else if (direction == Direction::West)
		{
			yStart = y - yLength / 2;
			yEnd = y + (yLength + 1) / 2;
			xStart = x - xLength;
		}
 
		if (!map.IsInBoundsX(xStart) || !map.IsInBoundsX(xEnd) || !map.IsInBoundsY(yStart) || !map.IsInBoundsY(yEnd))
			return false;
 
		if (!map.IsAreaUnused(xStart, yStart, xEnd, yEnd))
			return false;
 
		map.SetTiles(xStart, yStart, xEnd, yEnd, Tile::Wall);
		map.SetTiles(xStart + 1, yStart + 1, xEnd - 1, yEnd - 1, Tile::Floor);
 
		//std::cout << "Room: ( " << xStart << ", " << yStart << " ) to ( " << xEnd << ", " << yEnd << " )" << std::endl;
 
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
 
			int x = GetRandomInt(1, m_width - 2);
			int y = GetRandomInt(1, m_height - 2);
 
			if (map.GetTileTypeAt(x, y) != Tile::Wall && map.GetTileTypeAt(x, y) != Tile::Corridor)
				continue;
 
			if (map.IsAdjacentToTileType(x, y, Tile::Door))
				continue;
 
			if (map.GetTileTypeAt(x, y+1) == Tile::Floor || map.GetTileTypeAt(x, y+1) == Tile::Corridor)
			{
				if (MakeFeature(map, x, y, 0, -1, Direction::North))
					return true;
			}
			else if (map.GetTileTypeAt(x-1, y) == Tile::Floor || map.GetTileTypeAt(x-1, y) == Tile::Corridor)
			{
				if (MakeFeature(map, x, y, 1, 0, Direction::East))
					return true;
			}
			else if (map.GetTileTypeAt(x, y-1) == Tile::Floor || map.GetTileTypeAt(x, y-1) == Tile::Corridor)
			{
				if (MakeFeature(map, x, y, 0, 1, Direction::South))
					return true;
			}
			else if (map.GetTileTypeAt(x+1, y) == Tile::Floor || map.GetTileTypeAt(x+1, y) == Tile::Corridor)
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
				map.SetTile(x, y, Tile::Door);
 
				// Remove wall next to the door.
				map.SetTile(x + xmod, y + ymod, Tile::Floor);
 
				return true;
			}
 
			return false;
		}
		else
		{
			if (MakeCorridor(map, x + xmod, y + ymod, 6, direction))
			{
				map.SetTile(x, y, Tile::Door);
 
				return true;
			}
 
			return false;
		}
	}

	bool MapGenerator::MakeCorridor(Map& map, int x, int y, int maxLength, Direction direction) 
	{
		assert(x >= 0 && x < m_width);
		assert(y >= 0 && y < m_height);
 
		assert(maxLength > 0 && maxLength <= std::max(m_width, m_height));
 
		auto length = GetRandomInt(2, maxLength);
 
		auto xStart = x;
		auto yStart = y;
 
		auto xEnd = x;
		auto yEnd = y;
 
		if (direction == Direction::North)
			yStart = y - length;
		else if (direction == Direction::East)
			xEnd = x + length;
		else if (direction == Direction::South)
			yEnd = y + length;
		else if (direction == Direction::West)
			xStart = x - length;
 
		if (!map.IsInBoundsX(xStart) || !map.IsInBoundsX(xEnd) || !map.IsInBoundsY(yStart) || !map.IsInBoundsY(yEnd))
			return false;
 
		if (!map.IsAreaUnused(xStart, yStart, xEnd, yEnd))
			return false;
 
		map.SetTiles(xStart, yStart, xEnd, yEnd, Tile::Corridor);
 
		//std::cout << "Corridor: ( " << xStart << ", " << yStart << " ) to ( " << xEnd << ", " << yEnd << " )" << std::endl;
 
		return true;
	}

bool MapGenerator::MakeMap(Map & map)
{
	//Dig out a single room in the centre of the map
	MakeRoom(map, m_width / 2, m_height / 2, 8, 8, GetRandomDirection());

	
			for (auto features = 1; features < m_maxFeatures; ++features)
		{
			if (!MakeFeature(map))
			{
				std::cout << "Unable to place more features (placed " << features << ")." << std::endl;
				break;
			}
		}
 
		if (!MakeStairs(map, Tile::UpStairs))
			std::cout << "Unable to place up stairs." << std::endl;
 
		if (!MakeStairs(map, Tile::DownStairs))
			std::cout << "Unable to place down stairs." << std::endl;
	
		return true;
}

	bool MapGenerator::MakeStairs(Map& map, Tile tile)
	{
		auto tries = 0;
		auto maxTries = 10000;
 
		for ( ; tries != maxTries; ++tries)
		{
			int x = GetRandomInt( 1, m_width - 2);
			int y = GetRandomInt( 1, m_height - 2);
 
			if (!map.IsAdjacentToTileType(x, y, Tile::Floor) && !map.IsAdjacentToTileType (x, y, Tile::Corridor))
				continue;
 
			if (map.IsAdjacentToTileType(x, y, Tile::Door))
				continue;
 
			map.SetTile(x, y, tile);
 
			return true;
		}
 
		return false;
	}

	Direction MapGenerator::GetRandomDirection()
	{
		return Direction(rand() % 4);
	}

	int MapGenerator::GetRandomInt(int min, int max)
	{
		return (rand() % (max - min)) + min;
	}