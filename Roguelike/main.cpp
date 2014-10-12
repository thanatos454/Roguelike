#include <SFML/Graphics.hpp>
#include "Map.h"
#include "MapGenerator.h"

#include "StateManager.h"
#include "TitleState.h"
#include "GameState.h"
#include "InventoryMenu.h"

#define TILE_SIZE 16
#define TILES_WIDTH 80
#define TILES_HEIGH 40



int main()
{

	sf::Clock c;
	sf::Time frameTime;

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Roguelike");
	window.setFramerateLimit(60);

	StateManager	stateMan;
	TitleState*		title = new TitleState(window);
	GameState*		game = new GameState(window);
	InventoryMenu*	inventory = new InventoryMenu(window);

	stateMan.AddManagedState( inventory );
	stateMan.AddManagedState( game );
	stateMan.QueueStateChange( StateManager::ChangeType::Push, stateMan.AddManagedState( title ) );

	MapGenerator * mg = new MapGenerator(TILES_WIDTH,TILES_HEIGH,150,75,25,30);
	Map * map = mg->generateMap();
	map->LoadSFTiles(TILE_SIZE);

	while (window.isOpen())
	{
		tateMan.ProcessStateChange();

		sf::Event e;
		while (window.pollEvent(e))
		{
			switch( e.type )
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if( e.key.code == sf::Keyboard::Escape )
					window.close();
				break;
			}
			if (e.type == sf::Event::Closed)
			{
				window.close();
			}

			if (e.type == sf::Event::MouseButtonPressed)
			{
				if(e.mouseButton.button == sf::Mouse::Right)
				{
					map = mg->generateMap();
					map->LoadSFTiles(TILE_SIZE);
				}
			}

			stateMan.HandleInput(e);
			stateMan.Update(frameTime.asSeconds());
        }

		frameTime = c.getElapsedTime();
		c.restart();



        window.clear();
		for(int x = 0; x < TILES_WIDTH; x++)
		{
			for(int y = 0; y < TILES_HEIGH; y++)
			{
				window.draw(map->GetSFTile(x, y));
			}
		}

		stateMan.Render();
        window.display();
    }

    return 0;
}