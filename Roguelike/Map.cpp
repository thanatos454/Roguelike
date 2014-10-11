#include "Map.h"
#include <cassert>
#include <iostream>

Map::Map(int xSize, int ySize):
	data(xSize * ySize, Tile::Unused)
{
	this->xSize = xSize;
	this->ySize = ySize;
}

Map::~Map(){
}

void Map::SetTile(int x, int y, Tile type)
{
	assert(this->IsInBoundsX(x));
	assert(this->IsInBoundsY(y));

	data[x + xSize * y];
}

Tile Map::GetTile(int x, int y)
{
	assert(this->IsInBoundsX(x));
	assert(this->IsInBoundsY(y));

	return data[x + xSize * y];
}
bool  Map::IsInBoundsX(int x)
{
	return x >= 0 && x < xSize;
}

bool  Map::IsInBoundsY(int y)
{
	return y >= 0 &&  y < ySize;
}
bool  Map::IsAreaUnused(int xStart, int yStart, int xEnd, int yEnd)
{
		assert(IsInBoundsX(xStart) && IsInBoundsX(xEnd));
		assert(IsInBoundsY(yStart) && IsInBoundsY(yEnd));
 
		assert(xStart <= xEnd);
		assert(yStart <= yEnd);
 
		for (auto y = yStart; y != yEnd + 1; ++y)
			for (auto x = xStart; x != xEnd + 1; ++x)
				if (GetTile(x, y) != Tile::Unused)
					return false;
 
		return true;
}

bool  Map::IsAdjacentToTileType(int x, int y, Tile tileType)
{
	assert(IsInBoundsX(x - 1) && IsInBoundsX(x + 1));
	assert(IsInBoundsY(y - 1) && IsInBoundsY(y + 1));

		return 
			GetTile(x - 1, y) == tileType || GetTile(x + 1, y) == tileType ||
			GetTile(x, y - 1) == tileType || GetTile(x, y + 1) == tileType;
}

void  Map::testPrint()
{
	//Barrowed this code form roguebasin:
		for (auto y = 0; y != ySize; y++)
		{
			for (auto x = 0; x != xSize; x++)
			{
				switch(GetTile(x, y))
				{
				case Tile::Unused:
					std::cout << " ";
					break;
				case Tile::DirtWall:
					std::cout << "#";
					break;
				case Tile::DirtFloor:
					std::cout << ".";
					break;
				case Tile::Corridor:
					std::cout << ".";
					break;
				case Tile::Door:
					std::cout << "+";
					break;
				case Tile::UpStairs:
					std::cout << "<";
					break;
				case Tile::DownStairs:
					std::cout << ">";
					break;
				};
			}
 
			std::cout << std::endl;
		}
 
		std::cout << std::endl;

}