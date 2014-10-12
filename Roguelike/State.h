#pragma once

#include <string>

namespace sf
{
	class RenderWindow;
	class Event;
}

class StateManager;

class State
{
public:
	State( sf::RenderWindow& window, const std::string& name );
	virtual ~State(void);

	virtual void HandleInput(const sf::Event& e) = 0;
	virtual void Update(float dTime) = 0;
	virtual void Render(void) = 0;

	virtual void Pause(void) = 0;
	virtual void Unpause(void) = 0;

	unsigned int GetID() const;
	const std::string& GetName() const;

	void AssignManager( StateManager* manager );

protected:
	sf::RenderWindow&	m_window;
	StateManager*		m_manager;
	const std::string	m_name;

	unsigned int	m_id;
	bool			m_paused;

private:
	static unsigned int s_idCount;
};

