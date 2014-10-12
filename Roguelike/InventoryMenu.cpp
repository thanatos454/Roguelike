#include "InventoryMenu.h"
#include "StateManager.h"

#include <SFML/Graphics.hpp>

static sf::RectangleShape inventory;
static sf::Font font;
static sf::Text msg;

InventoryMenu::InventoryMenu( sf::RenderWindow& window ) : State(window, "InventoryMenu")
{
	inventory.setSize( sf::Vector2f( window.getSize().x*0.6f, window.getSize().y*0.6f ) );
	inventory.setFillColor(sf::Color::White);
	inventory.setOrigin( inventory.getSize().x/2, inventory.getSize().y/2 );
	inventory.setPosition( window.getSize().x/2.0f, window.getSize().y/2.0f );

	font.loadFromFile("arial.ttf");//no error checking here
	msg.setFont(font);
	msg.setString("You are living below the poverty line.");
	msg.setColor(sf::Color::Red);

	msg.setOrigin( msg.getGlobalBounds().width/2, msg.getGlobalBounds().height/2 );
	msg.setPosition( window.getSize().x/2.0f, window.getSize().y/2.0f );
}


InventoryMenu::~InventoryMenu(void)
{
}


void InventoryMenu::HandleInput(const sf::Event& e)
{

	if( (e.type == sf::Event::KeyReleased ) &&
	(e.key.code == sf::Keyboard::I) )
	{
		m_manager->QueueStateChange( StateManager::ChangeType::Pop, 0 );
	}
}
void InventoryMenu::Update(float dTime)
{
}
void InventoryMenu::Render(void)
{
		m_window.draw(inventory);
        m_window.draw(msg);
}

void InventoryMenu::Pause(void)
{
}
void InventoryMenu::Unpause(void)
{
}