#pragma once

#include <vector>

enum class Tile
{
	Unused,
	DirtWall,
	DirtFloor,
	Corridor,
	Door,
	UpStairs,
	DownStairs
};



class Map
{
public:
	Map(int xSize = 32, int ySize = 32);
	~Map();

	void SetTile(int x, int y, Tile type);
	void SetTiles(int xStart, int yStart, int xEnd, int yEnd, Tile cellType);
	Tile GetTile(int x, int y);
	bool IsInBoundsX(int x);
	bool IsInBoundsY(int y);
	bool IsAreaUnused(int xStart, int yStart, int xEnd, int yEnd);
	bool IsAdjacentToTileType(int x, int y, Tile tileType);
	void testPrint();

private:
	int xSize;
	int ySize;
	std::vector<Tile> data;

};