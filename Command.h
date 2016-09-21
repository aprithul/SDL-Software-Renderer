#pragma once
#include "Actor.h"

class Command
{
public:
	Command(void);
	virtual ~Command(void);
	virtual void execute() = 0;


};

