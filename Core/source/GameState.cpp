#include "D3DCore_impl.h"

//GameState Methods
void D3DCore_Impl::GState_Push(GameState* state)
{
	state->initialize();
	_states.push_back(state);
}

GameState* D3DCore_Impl::GState_Pop()
{
	GameState* state = 0;

	if (_states.size() > 0)
	{
		state = _states[_states.size()-1];
		_states.pop_back();
		state->shutdown();
		_popped_states.push_back( state );
	}

	return state;
}

void D3DCore_Impl::GState_PopAll()
{
	while (GState_Pop());
}

void D3DCore_Impl::delPopStates()
{
	GameState* state = 0;

	while (_popped_states.size() > 0)
	{
		state = _popped_states[_popped_states.size()-1];
		_popped_states.pop_back();
		delete state;
	}
}