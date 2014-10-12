#include "State.h"
//#include <SFML/Graphics.hpp>

unsigned int State::s_idCount = 1;

State::State( sf::RenderWindow& window, const std::string& name ) 
	: m_window(window), m_manager(0), m_name(name), m_paused(false)
{
	m_id = s_idCount++;
}


State::~State(void)
{
}

unsigned int State::GetID() const
{
	return m_id;
}

const std::string& State::GetName() const
{
	return m_name;
}

void State::AssignManager( StateManager* manager )
{
	m_manager = manager;
}