#include "MapGenerator.h"
#include <ctime> //time
#include <cstdlib> //rand, srand


enum class Direction
{
	North, South, East, West,
};

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

bool MapGenerator::generateMap()
{
	//TODO!!!!!!
	return false;
}