#pragma once

#include <vector>

class State;
namespace sf
{
	class Event;
}

class StateManager
{
public:
	StateManager(void);
	~StateManager(void);


	enum ChangeType
	{
		None,
		Swap,
		Push,
		Pop
	};

	void QueueStateChange( ChangeType type, unsigned int stateID );
	void ProcessStateChange();

	unsigned int	AddManagedState(State* state);
	State*			RemoveManagedState(unsigned int stateID);

	unsigned int	GetStateByName( const std::string& name );
	State*			GetStateByID(unsigned int stateID);

	void HandleInput(const sf::Event& e);
	void Update(float dTime);
	void Render(void);

	void Pause(void);
	void Unpause(void);

private:
	typedef std::vector<State*> StateList;

	StateList	m_activeStates;
	StateList	m_managedStates;

	ChangeType		m_stateChangeRequested;
	unsigned int	m_queuedStateID;

	bool			SwapState(unsigned int stateID);
	bool			PushState(unsigned int stateID);
	void			PopState(unsigned int stateID);
};

