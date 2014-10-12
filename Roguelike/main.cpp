#include <SFML/Graphics.hpp>


#include "StateManager.h"
#include "TitleState.h"
#include "GameState.h"
#include "InventoryMenu.h"



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

	stateMan.QueueStateChange( StateManager::ChangeType::Push, stateMan.AddManagedState( title ) );
	stateMan.AddManagedState( game );
	stateMan.AddManagedState( inventory );

	while (window.isOpen())
	{
		stateMan.ProcessStateChange();

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

			stateMan.HandleInput(e);
			stateMan.Update(frameTime.asSeconds());
        }

		frameTime = c.getElapsedTime();
		c.restart();

        window.clear();
		stateMan.Render();
        window.display();
    }

    return 0;
}