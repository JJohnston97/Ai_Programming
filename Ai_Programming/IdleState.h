#pragma once
#ifndef IDLESTATE_H
#define IDLESTATE_H

#include "State.h"

class IdleState : public State
{

private:
	int waitTime = 0;


public:
	IdleState(StateManager* manager);
	void update();



};








#endif // !IDLESTATE_H
