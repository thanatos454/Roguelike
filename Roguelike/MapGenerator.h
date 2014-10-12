#pragma once
#include "Map.h"



enum class Direction
{
	North, South, East, West,
};


class MapGenerator
{
public:
	MapGenerator(int xSize = 32, int ySize = 32, int maxFeatures = 150, int chanceRoom = 75, int chanceHall = 25, int seed = -1);
	~MapGenerator();
	void setSeed(int seed);
	int getSeed();

	Map * generateMap();
private:
	bool MakeMap(Map & map);
	bool MakeRoom(Map& map, int x, int y, int xMaxLength, int yMaxLength, Direction direction);
	bool MakeFeature(Map& map);
	bool MakeFeature(Map& map, int x, int y, int xmod, int ymod, Direction direction);
	bool MakeCorridor(Map& map, int x, int y, int maxLength, Direction direction) ;
	bool MakeStairs(Map& map, Tile tile);

	Direction GetRandomDirection();
	int GetRandomInt(int min, int max);

	int xSize;
	int ySize;
	int maxFeatures;
	int chanceRoom;
	int chanceHall;
	int seed;

};