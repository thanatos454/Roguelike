#include "Map.h"
#include <cassert>
#include <iostream>

Map::Map(int x, int y):
	data(x * y, Tile::Unused),
	sfTiles(x * y),
	xSize(x),
	ySize(y)
{
	if (x <= y)
		std::cout << "FU";
}

Map::~Map(){
}

void Map::LoadSFTiles(int tileSize)
{
	for(int x = 0; x < xSize; x++)
	{
		for(int y = 0; y < ySize; y++)
		{
			sf::RectangleShape tile(sf::Vector2f(tileSize,tileSize));
			tile.setPosition(x*tileSize, y*tileSize);

			switch(GetTile(x, y))
				{
				case Tile::Unused:	
					tile.setFillColor(sf::Color::Black);
					break;
				case Tile::DirtWall:
					tile.setFillColor(sf::Color::Red);
					break;
				case Tile::DirtFloor:
					tile.setFillColor(sf::Color::Blue);
					break;
				case Tile::Corridor:
					tile.setFillColor(sf::Color::Cyan);
					break;
				case Tile::Door:
					tile.setFillColor(sf::Color::Magenta);
					break;
				case Tile::UpStairs:
					tile.setFillColor(sf::Color::White);
					break;
				case Tile::DownStairs:
					tile.setFillColor(sf::Color::Yellow);
					break;
				};

			sfTiles[x + xSize * y] = tile;
		}

	}
}

sf::RectangleShape Map::GetSFTile(int x, int y)
{
	return sfTiles[x + xSize * y];
}


void Map::SetTile(int x, int y, Tile type)
{
	assert(this->IsInBoundsX(x));
	assert(this->IsInBoundsY(y));

	data[x + xSize * y] = type;
}

void Map::SetTiles(int xStart, int yStart, int xEnd, int yEnd, Tile cellType)
{
	assert(IsInBoundsX(xStart) && IsInBoundsX(xEnd));
	assert(IsInBoundsY(yStart) && IsInBoundsY(yEnd));
 
	assert(xStart <= xEnd);
	assert(yStart <= yEnd);
 
		for (int y = yStart; y < yEnd + 1; ++y)
		{
			for (int x = xStart; x < xEnd + 1; ++x)
			{
				this->SetTile(x, y, cellType);
			}
		}
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
		if(IsInBoundsX(xStart) && IsInBoundsX(xEnd) && IsInBoundsY(yStart) && IsInBoundsY(yEnd))
		{
 
			assert(xStart <= xEnd);
			assert(yStart <= yEnd);
 
			for (auto y = yStart; y != yEnd + 1; ++y)
				for (auto x = xStart; x != xEnd + 1; ++x)
					if (GetTile(x, y) != Tile::Unused)
						return false;
 
			return true;
		}
		else
		{
			return false;
		}
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
		for (int y = 0; y < ySize; y++)
		{
			for (int x = 0; x < xSize; x++)
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