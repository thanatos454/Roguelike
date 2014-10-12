#include <SFML/Graphics.hpp>
#include "Map.h"
#include "MapGenerator.h"

#define TILE_SIZE 16
#define TILES_WIDTH 80
#define TILES_HEIGH 40


int main()
{
    sf::RenderWindow window(sf::VideoMode(TILE_SIZE * TILES_WIDTH, TILE_SIZE * TILES_HEIGH), "SFML works!");

	
	MapGenerator * mg = new MapGenerator(TILES_WIDTH,TILES_HEIGH,150,75,25,30);
	Map * map = mg->generateMap();
	map->LoadSFTiles(TILE_SIZE);

	

    while (window.isOpen())
    {
		

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			if (event.type == sf::Event::MouseButtonPressed)
				if(event.mouseButton.button == sf::Mouse::Right)
				{
					map = mg->generateMap();
					map->LoadSFTiles(TILE_SIZE);
				}

        }

        window.clear();

		for(int x = 0; x < TILES_WIDTH; x++)
		{
			for(int y = 0; y < TILES_HEIGH; y++)
			{
				window.draw(map->GetSFTile(x, y));
			}
		}

        window.display();
    }

    return 0;
}