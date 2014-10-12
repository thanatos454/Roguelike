#include "TitleState.h"
#include "StateManager.h"

#include <SFML/Graphics.hpp>

static sf::Font font;
static sf::Text msg;

TitleState::TitleState( sf::RenderWindow& window ) : State(window, "TitleState")
{
	font.loadFromFile("arial.ttf");//no error checking here
	msg.setFont(font);
	msg.setString("Hit <Enter> to start.");
	msg.setColor(sf::Color::Red);

	msg.setOrigin( msg.getGlobalBounds().width/2, msg.getGlobalBounds().height/2 );
	msg.setPosition( window.getSize().x/2.0f, window.getSize().y/2.0f );
}


TitleState::~TitleState(void)
{
}


void TitleState::HandleInput(const sf::Event& e)
{
	if( (e.type == sf::Event::KeyReleased ) &&
		(e.key.code == sf::Keyboard::Return) )
	{
		unsigned int id = m_manager->GetStateByName("GameState");
		m_manager->QueueStateChange( StateManager::ChangeType::Swap, id);
	}




	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	//{
	//	unsigned int id = m_manager->GetStateByName("GameState");
	//	m_manager->QueueStateChange( StateManager::ChangeType::Swap, id);
	//}
}
void TitleState::Update(float dTime)
{
}
void TitleState::Render(void)
{
        m_window.draw(msg);
}

void TitleState::Pause(void)
{
}
void TitleState::Unpause(void)
{
}