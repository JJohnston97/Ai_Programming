#include "IdleState.h"
#include "StateMananger.h"
#include <Windows.h>
#include <iostream>

IdleState::IdleState(StateManager* manager) : State(manager)
{

	waitTime = 20;

}

void IdleState::update()
{
	if (waitTime <= 0)
	{
		
		//manager->setState(new SearchState(manager));
		std::cout << "do something" << std::endl;
		manager->setState(NULL);
	}
	else
	{

		waitTime--;
		std::cout << "WORKING" << std::endl;

	}

}
