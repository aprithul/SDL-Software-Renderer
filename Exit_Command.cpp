#include "Exit_Command.h"


Exit_Command::Exit_Command(bool* is_running)
{
	this->is_running = is_running;

}


Exit_Command::~Exit_Command(void)
{
}



void Exit_Command::execute() 
{
	*(this->is_running) = false;
}