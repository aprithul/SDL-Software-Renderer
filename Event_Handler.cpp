#include "Event_Handler.h"


Event_Handler::Event_Handler(void)
{

}


Event_Handler::~Event_Handler(void)
{
	delete escape_key_event;
	delete cross_window_event;
}

void Event_Handler::set_exit_command(bool*is_running)
{
	escape_key_event = new Exit_Command(is_running);
	cross_window_event = new Exit_Command(is_running);
}


Command* Event_Handler::handle_event()
{
	while(SDL_PollEvent(&sdl_event))
	{
		switch(sdl_event.type)
		{
		case SDL_QUIT:
			return cross_window_event;
			break;
		case SDL_KEYDOWN:
			switch(sdl_event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				return escape_key_event;
			}
		}
	}
}