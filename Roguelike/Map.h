#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

enum class Tile
{
	Unused,
	Wall,
	Floor,
	Corridor,
	Door,
	UpStairs,
	DownStairs,
	Count
};

namespace sf
{
	class RenderWindow;
	class RectangleShape;
}

class Map
{
public:
	Map(int width = 32, int height = 32, int size = 16);
	~Map();

	void Render( sf::RenderWindow& target );

	void SetTile(int x, int y, Tile type);
	void SetTiles(int xStart, int yStart, int xEnd, int yEnd, Tile type);

	Tile GetTileTypeAt(int x, int y);

	bool IsInBoundsX(int x);
	bool IsInBoundsY(int y);
	bool IsAreaUnused(int xStart, int yStart, int xEnd, int yEnd);
	bool IsAdjacentToTileType(int x, int y, Tile tileType);

	void TestPrint();

private:
	sf::RectangleShape& GetTile(int x, int y);
	void GenerateTileset(int tileSize);

	int					m_tileSize;
	int					m_width;
	int					m_height;
	std::vector<Tile>	m_data;

	static std::vector<sf::RectangleShape>	s_tileset;
};