#pragma once
#include "Map.h"



enum class Direction
{
	North, South, East, West,
};


class MapGenerator
{
public:
	MapGenerator(int width = 32, int height = 32, int maxFeatures = 150, int chanceRoom = 75, int chanceHall = 25, int seed = -1);
	~MapGenerator();
	void SetSeed(int seed);
	int GetSeed();

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

	int m_width;
	int m_height;
	int m_maxFeatures;
	int m_chanceRoom;
	int m_chanceHall;
	int m_seed;

};