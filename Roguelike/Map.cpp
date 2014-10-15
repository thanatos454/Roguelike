#include "Map.h"
#include <cassert>
#include <iostream>
#include <SFML\Graphics.hpp>

std::vector<sf::RectangleShape> Map::s_tileset;

Map::Map(int width, int height, int size) :
	m_tileSize(size),
	m_width(width),
	m_height(height),
	m_data(width * height, Tile::Unused)
{
	if( s_tileset.empty() )
	{
		GenerateTileset(size);
	}
}

Map::~Map(){
}

void Map::Render( sf::RenderWindow& target )
{
	for(int x = 0; x < m_width; x++)
	{
		for(int y = 0; y < m_height; y++)
		{
			target.draw(GetTile(x, y));
		}
	}
}

void Map::SetTile(int x, int y, Map::Tile type)
{
	assert(IsInBoundsX(x));
	assert(IsInBoundsY(y));

	m_data[x + m_width * y] = type;
}

void Map::FillRect(int left, int top, int right, int bottom, Tile type)
{
	assert(IsInBoundsX(left) && IsInBoundsX(right));
	assert(IsInBoundsY(top) && IsInBoundsY(bottom));
 
	assert(left <= right);
	assert(top <= bottom);
 
		for (int y = top; y < bottom + 1; ++y)
		{
			for (int x = left; x < right + 1; ++x)
			{
				this->SetTile(x, y, type);
			}
		}
}

Map::Tile Map::GetTileTypeAt(int x, int y)
{
	assert(IsInBoundsX(x));
	assert(IsInBoundsY(y));

	return m_data[x + m_width * y];
}

bool  Map::IsInBoundsX(int x)
{
	return x >= 0 && x < m_width;
}
bool  Map::IsInBoundsY(int y)
{
	return y >= 0 &&  y < m_height;
}
bool  Map::IsAreaUnused(int left, int top, int right, int bottom)
{
		if(IsInBoundsX(left) && IsInBoundsX(right) && IsInBoundsY(top) && IsInBoundsY(bottom))
		{
 
			assert(left <= right);
			assert(top <= bottom);
 
			for (auto y = top; y != bottom + 1; ++y)
				for (auto x = left; x != right + 1; ++x)
					if (GetTileTypeAt(x, y) != Tile::Unused)
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
			GetTileTypeAt(x - 1, y) == tileType || GetTileTypeAt(x + 1, y) == tileType ||
			GetTileTypeAt(x, y - 1) == tileType || GetTileTypeAt(x, y + 1) == tileType;
}

void  Map::TestPrint()
{
	//Barrowed this code form roguebasin:
	//Borrowed?  Well, just be sure to return it. :P
		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				switch(GetTileTypeAt(x, y))
				{
				case Tile::Unused:
					std::cout << " ";
					break;
				case Tile::Wall:
					std::cout << "#";
					break;
				case Tile::Floor:
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

sf::RectangleShape& Map::GetTile(int x, int y)
{
	sf::RectangleShape& tile = s_tileset[static_cast<int>(m_data[x + m_width * y])];
	tile.setPosition( x*m_tileSize, y*m_tileSize );

	return tile;
}

void Map::GenerateTileset(int tileSize)
{
	s_tileset.resize(static_cast<int>(Tile::Count));
	//case Tile::Unused:	
	s_tileset[0].setSize(sf::Vector2f(static_cast<float>(tileSize),static_cast<float>(tileSize)));
	s_tileset[0].setFillColor(sf::Color::Black);
	//case Tile::Wall:
	s_tileset[1].setSize(sf::Vector2f(static_cast<float>(tileSize),static_cast<float>(tileSize)));
	s_tileset[1].setFillColor(sf::Color::Red);
	//case Tile::Floor:
	s_tileset[2].setSize(sf::Vector2f(static_cast<float>(tileSize),static_cast<float>(tileSize)));
	s_tileset[2].setFillColor(sf::Color::Blue);
	//case Tile::Corridor:
	s_tileset[3].setSize(sf::Vector2f(static_cast<float>(tileSize),static_cast<float>(tileSize)));
	s_tileset[3].setFillColor(sf::Color::Cyan);
	//case Tile::Door:
	s_tileset[4].setSize(sf::Vector2f(static_cast<float>(tileSize),static_cast<float>(tileSize)));
	s_tileset[4].setFillColor(sf::Color::Magenta);
	//case Tile::UpStairs:
	s_tileset[5].setSize(sf::Vector2f(static_cast<float>(tileSize),static_cast<float>(tileSize)));
	s_tileset[5].setFillColor(sf::Color::White);
	//case Tile::DownStairs:
	s_tileset[6].setSize(sf::Vector2f(static_cast<float>(tileSize),static_cast<float>(tileSize)));
	s_tileset[6].setFillColor(sf::Color::Yellow);
}