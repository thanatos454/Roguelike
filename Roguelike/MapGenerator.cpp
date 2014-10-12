#include "MapGenerator.h"
#include "Map.h"
#include <cassert>
#include <ctime> //time
#include <cstdlib> //rand, srand
#include <iostream>


//////////
//  xSize & ySize are the number of tiles
//  Set seed to -1 for a random map.
//////////
MapGenerator::MapGenerator(int xSize, int ySize, int maxFeatures, int chanceRoom, int chanceHall, int seed)
{
	this->xSize = xSize;
	this->ySize = ySize;
	this->maxFeatures = maxFeatures;
	this->chanceRoom = chanceRoom;
	this->chanceHall = chanceHall;



	if (seed == -1)
		seed = time(0);

	this->setSeed(seed);


}

MapGenerator::~MapGenerator()
{

}

int MapGenerator::getSeed()
{
	return this->seed;
}
void MapGenerator::setSeed(int seed)
{
	this->seed = seed;
	srand(seed);
}

Map * MapGenerator::generateMap()
{
	assert(maxFeatures > 0);
	assert(xSize > 3 );
	assert(ySize > 3);

	Map * map = new Map(xSize, ySize);

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
 
		map.SetTiles(xStart, yStart, xEnd, yEnd, Tile::DirtWall);
		map.SetTiles(xStart + 1, yStart + 1, xEnd - 1, yEnd - 1, Tile::DirtFloor);
 
		std::cout << "Room: ( " << xStart << ", " << yStart << " ) to ( " << xEnd << ", " << yEnd << " )" << std::endl;
 
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
 
			int x = GetRandomInt(1, xSize - 2);
			int y = GetRandomInt(1, ySize - 2);
 
			if (map.GetTile(x, y) != Tile::DirtWall && map.GetTile(x, y) != Tile::Corridor)
				continue;
 
			if (map.IsAdjacentToTileType(x, y, Tile::Door))
				continue;
 
			if (map.GetTile(x, y+1) == Tile::DirtFloor || map.GetTile(x, y+1) == Tile::Corridor)
			{
				if (MakeFeature(map, x, y, 0, -1, Direction::North))
					return true;
			}
			else if (map.GetTile(x-1, y) == Tile::DirtFloor || map.GetTile(x-1, y) == Tile::Corridor)
			{
				if (MakeFeature(map, x, y, 1, 0, Direction::East))
					return true;
			}
			else if (map.GetTile(x, y-1) == Tile::DirtFloor || map.GetTile(x, y-1) == Tile::Corridor)
			{
				if (MakeFeature(map, x, y, 0, 1, Direction::South))
					return true;
			}
			else if (map.GetTile(x+1, y) == Tile::DirtFloor || map.GetTile(x+1, y) == Tile::Corridor)
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
 
		if (chance <= chanceRoom)
		{
			if (MakeRoom(map, x + xmod, y + ymod, 8, 6, direction))
			{
				map.SetTile(x, y, Tile::Door);
 
				// Remove wall next to the door.
				map.SetTile(x + xmod, y + ymod, Tile::DirtFloor);
 
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
		assert(x >= 0 && x < xSize);
		assert(y >= 0 && y < ySize);
 
		assert(maxLength > 0 && maxLength <= std::max(xSize, ySize));
 
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
	MakeRoom(map, xSize / 2, ySize / 2, 8, 8, GetRandomDirection());

	
			for (auto features = 1; features < maxFeatures; ++features)
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
			int x = GetRandomInt( 1, xSize - 2);
			int y = GetRandomInt( 1, ySize - 2);
 
			if (!map.IsAdjacentToTileType(x, y, Tile::DirtFloor) && !map.IsAdjacentToTileType (x, y, Tile::Corridor))
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