#pragma once
#include "state.h"

class InventoryMenu :
	public State
{
public:
	InventoryMenu( sf::RenderWindow& window );
	virtual ~InventoryMenu(void);

	virtual void HandleInput(const sf::Event& e);
	virtual void Update(float dTime);
	virtual void Render(void);

	virtual void Pause(void);
	virtual void Unpause(void);

private:


};