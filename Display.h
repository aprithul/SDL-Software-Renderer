#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
#include "Vector.h"

class Display
{
public:
	Display(int width, int height, const std::string& title);
	virtual ~Display(void);


	void clear_screen(Uint8 r,Uint8 g, Uint8 b, Uint8 a);
	void swap_buffers();

	void draw_point(Vector position, Uint8 r,Uint8 g, Uint8 b, Uint8 a);
	void draw_point(float x, float y, Uint8 r,Uint8 g, Uint8 b, Uint8 a);
	void draw_line(Vector point_1, Vector point_2,int r, int g, int b, int a);
	void draw_triangle_wireframe(Vector point_min, Vector point_mid, Vector point_max, int r, int g, int b, int a);
	void draw_triangle_filled(Vector point_1, Vector point_2, Vector point_3,  int r, int g, int b, int a);
	void draw_scan_buffer(int min_y, int max_y,int r, int g, int b, int a);


	const int SCREEN_WIDTH;
	const int SCREEN_HEIGHT;


private:

	void switch_to_octant_zero_from(int octant, Vector* point );
	void switch_from_octant_zero_to(int octant, Vector* point );
	float get_x_on_line_at_y(Vector point_1, float inverse_slope, float y);
	void scan_buffer_line(Vector point_1, Vector point_2,int handedness);
	//The window we'll be rendering to
	SDL_Window* window ;
	// The renderer associated with the window
	SDL_Renderer* renderer;
	// scan buffer
	int scan_buffer[600][2];
	SDL_Point bitmap[48000];
	int sdl_point_count;

};

