#include "GameState.h"
#include "StateManager.h"

#include <SFML/Graphics.hpp>

static sf::Font font;
static sf::Text msg;

GameState::GameState( sf::RenderWindow& window ) : State(window, "GameState")
{
	font.loadFromFile("arial.ttf");//no error checking here
	msg.setFont(font);
	msg.setString("You are having the time of your life...  \nHit <i> to toggle your inventory.");
	msg.setColor(sf::Color::Red);

	msg.setOrigin( msg.getGlobalBounds().width/2, msg.getGlobalBounds().height/2 );
	msg.setPosition( window.getSize().x/2.0f, window.getSize().y/2.0f );
}


GameState::~GameState(void)
{
}


void GameState::HandleInput(const sf::Event& e)
{
	if( (e.type == sf::Event::KeyReleased ) &&
	(e.key.code == sf::Keyboard::I) )
	{
		unsigned int id = m_manager->GetStateByName("InventoryMenu");
		m_manager->QueueStateChange( StateManager::ChangeType::Push, id );
	}
}
void GameState::Update(float dTime)
{
}
void GameState::Render(void)
{
        m_window.draw(msg);
}

void GameState::Pause(void)
{
}
void GameState::Unpause(void)
{
}