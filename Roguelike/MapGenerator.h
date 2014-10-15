#pragma once
#include "Map.h"

class MapGenerator
{
	enum Direction
	{
		North, South, East, West,
	};

public:
	MapGenerator(int maxFeatures = 150, int chanceRoom = 75, int chanceHall = 25, int seed = -1);
	~MapGenerator();
	void SetSeed(int seed);
	int GetSeed();

	void Generate( Map& map );

private:
	bool MakeMap(Map& map);
	bool MakeRoom(Map& map, int x, int y, int maxWidth, int maxHeight, Direction direction);
	bool MakeFeature(Map& map);
	bool MakeFeature(Map& map, int x, int y, int xmod, int ymod, Direction direction);
	bool MakeCorridor(Map& map, int x, int y, int maxLength, Direction direction) ;
	bool MakeStairs(Map& map, Map::Tile tile);

	Direction GetRandomDirection();
	int GetRandomInt(int min, int max);

	int m_maxFeatures;
	int m_chanceRoom;
	int m_chanceHall;
	int m_seed;

};