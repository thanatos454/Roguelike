#pragma once
#include "state.h"

class GameState :
	public State
{
public:
	GameState( sf::RenderWindow& window );
	virtual ~GameState(void);

	virtual void HandleInput(const sf::Event& e);
	virtual void Update(float dTime);
	virtual void Render(void);

	virtual void Pause(void);
	virtual void Unpause(void);

private:


};