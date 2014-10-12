#include "StateManager.h"
#include "State.h"

//I just loop through the vectors to find the ID's because for these small games there won't be that many states
//in the future an optimization would be to use associative arrays to remove the need to search through vectors 
//manually to find the appropriate states.

StateManager::StateManager(void) : m_stateChangeRequested(ChangeType::None), m_queuedStateID(0)
{
}


StateManager::~StateManager(void)
{
	for( StateList::iterator it = m_managedStates.begin();
		it != m_managedStates.end();
		++it )
	{
		delete (*it);
	}
}

void StateManager::QueueStateChange( ChangeType type, unsigned int stateID )
{
	if( type != ChangeType::None )
	{
		m_queuedStateID = stateID;
		m_stateChangeRequested = type;
	}
}
void StateManager::ProcessStateChange()
{
	switch( m_stateChangeRequested )
	{
	case ChangeType::Swap:
		SwapState(m_queuedStateID);
		break;
	case ChangeType::Push:
		PushState(m_queuedStateID);
		break;
	case ChangeType::Pop:
		PopState(m_queuedStateID);
		break;
	case ChangeType::None:
	default:
		return;
	}
	m_stateChangeRequested = ChangeType::None;
	m_queuedStateID = 0;
}

unsigned int StateManager::AddManagedState(State* state)
{
	if( state != GetStateByID(state->GetID()) )
	{
		state->AssignManager(this);
		m_managedStates.push_back(state);
	}	
	return state->GetID();
}


State* StateManager::RemoveManagedState(unsigned int stateID)
{
	State* s = NULL;

	//Remove from managed list
	for( StateList::iterator it = m_managedStates.begin();
		it != m_managedStates.end();
		++it )
	{
		if( (*it)->GetID() == stateID )
		{
			s = *it;
			s->AssignManager(0);
			m_managedStates.erase( it );

			//Removes from active list
			QueueStateChange(ChangeType::Pop, stateID);
			break;
		}
	}

	return s;
}

unsigned int StateManager::GetStateByName( const std::string& name )
{
	for( StateList::iterator it = m_managedStates.begin();
		it != m_managedStates.end();
		++it )
	{
		if( (*it)->GetName() == name )
			return (*it)->GetID();
	}
	return 0;
}

State* StateManager::GetStateByID(unsigned int stateID)
{
	for( StateList::iterator it = m_managedStates.begin();
		it != m_managedStates.end();
		++it )
	{
		if( (*it)->GetID() == stateID )
			return *it;
	}
	return NULL;
}



void StateManager::HandleInput(const sf::Event& e)
{
	for( StateList::iterator it = m_activeStates.begin();
		it != m_activeStates.end();
		it++ )
	{
		(*it)->HandleInput(e);
	}
}
void StateManager::Update(float dTime)
{
	for( StateList::iterator it = m_activeStates.begin();
		it != m_activeStates.end();
		it++ )
	{
		(*it)->Update(dTime);
	}
}
void StateManager::Render(void)
{
	for( StateList::iterator it = m_activeStates.begin();
		it != m_activeStates.end();
		it++ )
	{
		(*it)->Render();
	}
}

void StateManager::Pause(void)
{

}
void StateManager::Unpause(void)
{

}



bool StateManager::SwapState(unsigned int stateID)
{
	PopState(0);
	return PushState(stateID);
}


bool StateManager::PushState(unsigned int stateID)
{
	//If state is already in activeState list it is removed to be put at the end of the list
	for( StateList::iterator it = m_activeStates.begin();
		it != m_activeStates.end();
		++it )
	{
		if( (*it)->GetID() == stateID )
		{
			m_activeStates.erase( it );
			break;
		}
	}
	State* s = GetStateByID(stateID);
	if( NULL != s )
	{
		m_activeStates.push_back(s);
		return true;
	}
	
	return false;
}

void StateManager::PopState(unsigned int stateID)
{
	if( !m_activeStates.empty() )
	{
		if( stateID == 0 )
		{
			m_activeStates.pop_back();
		}
		else
		{
			//Remove from active list
			for( StateList::iterator it = m_activeStates.begin();
				it != m_activeStates.end();
				++it )
			{
				if( (*it)->GetID() == stateID )
				{
					m_activeStates.erase( it );
					break;
				}
			}
		}
	}
}