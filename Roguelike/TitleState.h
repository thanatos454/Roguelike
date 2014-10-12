#pragma once
#include "State.h"

class TitleState :
	public State
{
public:
	TitleState( sf::RenderWindow& window );
	virtual ~TitleState(void);

	virtual void HandleInput(const sf::Event& e);
	virtual void Update(float dTime);
	virtual void Render(void);

	virtual void Pause(void);
	virtual void Unpause(void);

private:


};

