#pragma once

#include <SDL2/SDL.h>
#include "Command.h"
#include "Exit_Command.h"

class Event_Handler
{
public:
	Event_Handler(void);
	virtual ~Event_Handler(void);

	void set_exit_command(bool* is_running);
	Command* handle_event();


private:

	// receives all sdl events
	SDL_Event sdl_event;

	// all button commands declarations go here
	Command* escape_key_event;
	Command* cross_window_event;

};

