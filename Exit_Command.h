#pragma once
#include "Command.h"
#include <iostream>

class Exit_Command :public Command
{

public:
	Exit_Command(bool* is_running);
	virtual ~Exit_Command(void);

	using Command::execute;
	virtual void execute() override;

private:
	bool* is_running;


};

