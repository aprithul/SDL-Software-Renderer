#include <iostream>
#include <SDL2/SDL.h>
#include "Display.h"
#include "Event_Handler.h"


bool is_running = true;		// to determine when to quit game
SDL_Event sdl_event;		//event handler
Event_Handler event_handler;



int main( int argc, char* args[] )
{
	// initial setups-------------------------------------------------

	Display display(800,600,"hello");			// reference to a display oboject, cotains all rendering stuff

	// set all required commands here
	event_handler.set_exit_command(&is_running);

	// end setup--------------------------------------------------------


	unsigned int last_time = SDL_GetTicks() ,current_time = 0;
	unsigned int total_time = 0;
	unsigned int fps = 0;

	// main loop
	while(is_running == true)
	{
		// execute any event that occured
		Command* command = event_handler.handle_event();

		if(command != NULL)
			command->execute();

		// render
		display.clear_screen(0,0,0,255);
		//display.draw_point(glm::vec2(display.SCREEN_WIDTH/2,display.SCREEN_HEIGHT/2),255,0,0,255);
		//display.draw_line(glm::vec2(display.SCREEN_WIDTH/2, display.SCREEN_HEIGHT/2),glm::vec2(display.SCREEN_WIDTH/2 - 100, display.SCREEN_HEIGHT/2 + 100),255,0,0,255);
		display.draw_triangle_filled( Vector(100,100,0), Vector(150,200,0), Vector(200,30,0),255,0,255,255);

		display.swap_buffers();

		current_time = SDL_GetTicks();
		//std::cout<< ((current_time - last_time))/(float)1000 <<std::endl;
		total_time += (current_time - last_time);
		if(total_time>=1000)
		{
			total_time -=1000;
			std::cout<<fps<<std::endl;
			fps = 0;
		}
		else
			fps++;

		last_time = current_time;


	}

	// ran successfully
	return 0;
}









