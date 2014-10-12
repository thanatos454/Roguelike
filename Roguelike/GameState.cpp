#include "GameState.h"
#include <SFML/Graphics.hpp>


#include "StateManager.h"
#include "Map.h"
#include "MapGenerator.h"

#define TILE_SIZE 16
#define TILES_WIDTH 80
#define TILES_HEIGH 40


GameState::GameState( sf::RenderWindow& window ) : State(window, "GameState"), m_mapGen(new MapGenerator(TILES_WIDTH,TILES_HEIGH,150,75,25,30))
{

	m_map = m_mapGen->generateMap();
	m_map->LoadSFTiles(TILE_SIZE);
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
		delete m_map;
		m_map = m_mapGen->generateMap();
		m_map->LoadSFTiles(TILE_SIZE);
	}
}
void GameState::Update(float dTime)
{
}
void GameState::Render(void)
{
	for(int x = 0; x < TILES_WIDTH; x++)
	{
		for(int y = 0; y < TILES_HEIGH; y++)
		{
			m_window.draw(m_map->GetSFTile(x, y));
		}
	}
}

void GameState::Pause(void)
{
}
void GameState::Unpause(void)
{
}