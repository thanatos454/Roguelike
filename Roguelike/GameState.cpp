#include "GameState.h"
#include <SFML/Graphics.hpp>


#include "StateManager.h"
#include "Map.h"
#include "MapGenerator.h"

#define TILE_SIZE 16
#define MAP_WIDTH 64
#define MAP_HEIGHT 48


GameState::GameState( sf::RenderWindow& window ) : State(window, "GameState"), m_map(new Map(MAP_WIDTH, MAP_HEIGHT, TILE_SIZE)), m_mapGen(new MapGenerator(150,75,25,30))
{
	m_mapGen->Generate(*m_map);
}


GameState::~GameState(void)
{
	delete m_map;
	delete m_mapGen;
}


void GameState::HandleInput(const sf::Event& e)
{
	if( (e.type == sf::Event::KeyReleased ) &&
	(e.key.code == sf::Keyboard::I) )
	{
		unsigned int id = m_manager->GetStateByName("InventoryMenu");
		m_manager->QueueStateChange( StateManager::ChangeType::Push, id );
	}

	if( (e.type == sf::Event::MouseButtonPressed) &&
	(e.mouseButton.button == sf::Mouse::Right) )
	{
		m_mapGen->Generate(*m_map);
	}
}
void GameState::Update(float dTime)
{
}
void GameState::Render(void)
{
	m_map->Render(m_window);
}

void GameState::Pause(void)
{
}
void GameState::Unpause(void)
{
}
