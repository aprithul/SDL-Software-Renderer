#include "Display.h"

Display::Display(int width, int height, const std::string& title):SCREEN_HEIGHT(height),SCREEN_WIDTH(width)
{
	//Initialize SDL vidoe module
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        std::cout<<"SDL could not initialize! SDL_Error: %s\n "<< SDL_GetError()<<std::endl;
    }
	else
	{
		//Create window
        window = SDL_CreateWindow( "RnderC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN );
        if( window == NULL )
		{
			std::cout<<"Window couldn't be created "<<SDL_GetError()<<std::endl;
		}
		else
			// create renderer
			renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
	}

	sdl_point_count = 0;
}


Display::~Display(void)
{
	//Destroy window
	SDL_DestroyWindow( window );

	//Quit SDL subsystems
	SDL_Quit();
}

void Display::swap_buffers()
{
	// send all the points at once to the renderer
	SDL_RenderDrawPoints(renderer, bitmap,sdl_point_count);

	// show rendered window
	SDL_RenderPresent(renderer);

	// reset for next frame
	sdl_point_count = 0;

}

void Display::clear_screen(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	// clear the screen with the provided color
	SDL_SetRenderDrawColor(renderer,r,g,b,a);
}


void Display::draw_point(Vector position, Uint8 r,Uint8 g, Uint8 b, Uint8 a)
{
	// set render color
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Point p = {position.x,position.y};
	bitmap[sdl_point_count++] = p;

	// draw a pixel
	//SDL_RenderDrawPoint(renderer, position.x, Display::SCREEN_HEIGHT- position.y);

}


void Display::draw_point(float x, float y, Uint8 r,Uint8 g, Uint8 b, Uint8 a)
{
	// set red color
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	// draw a pixel
	SDL_Point p = {x,y};
	bitmap[sdl_point_count++] = p;
	//SDL_RenderDrawPoint(renderer, x+0.5f, Display::SCREEN_HEIGHT- y - 0.5f );

}

void Display::draw_line(Vector point_1, Vector point_2, int r, int g, int b, int a)
{
	// translate point_1 to the center of the co-ordinate system
	Vector offset(point_1);
	point_1.x = 0;
	point_1.y = 0;
	point_2.x -= offset.x;
	point_2.y -= offset.y;

	// find which octant pont_2 is in
	int octant = 0;
	if(point_2.x>0)
	{	if(point_2.y>=0)
		{
			if(point_2.x>=point_2.y)
				octant = 0;
			else
				octant = 1;
		}
		else
		{
			if (point_2.x>= ::abs(point_2.y))
				octant = 7;
			else
				octant = 6;
		}
	}
	else
	{
		if(point_2.y>=0)
		{
			if(std::abs(point_2.x)>=point_2.y)
				octant = 3;
			else
				octant = 2;
		}
		else
		{
			if (std::abs(point_2.x) >= std::abs(point_2.y))
				octant = 4;
			else
				octant = 5;
		}
	}

	// map point_2 to calculatable octant , the octant 0
	switch_to_octant_zero_from(octant, &point_2);

	// bresenham's line algorithm

	Vector point(point_1);
	int dx = point_2.x - point_1.x;
	int dy = point_2.y - point_1.y;
	int dT = 2*(dy - dx);
	int dS = 2 * dy;
	int d  = (2*dy) - dx;

	Vector octate_corrected_point(point_1);
	switch_from_octant_zero_to(octant ,&octate_corrected_point);
	Display::draw_point(octate_corrected_point + offset, r, g, b, a);

	while(point.x < point_2.x)
	{
		point.x++;

		if(d<0)
			d = d+dS;
		else
		{
			point.y++;
			d = d + dT;
		}

		octate_corrected_point.x = point.x;
		octate_corrected_point.y = point.y;
		switch_from_octant_zero_to(octant,&octate_corrected_point);
		Display::draw_point(octate_corrected_point + offset, r, g, b, a);

	}
}

void Display::draw_triangle_wireframe(Vector point_1, Vector point_2, Vector point_3, int r, int g, int b, int a)
{
	// sort based on y
	if(point_1.y > point_2.y) std::swap(point_1,point_2);
	if(point_1.y > point_3.y) std::swap(point_1,point_3);
	if(point_2.y > point_3.y) std::swap(point_2,point_3);

	// get a fourth point on the line ( point_3 , point_1) with the same y value as point_2
	//Vector point_4( get_x_on_line_at_y(point_1, (point_1.x-point_3.x)/(float)(point_1.y-point_3.y),point_2.y) ,point_2.y);


	// draw triangles
	draw_line(point_1,point_2,r,g,b,a);
	//draw_line(point_1,point_4,r,g,b,a);
	draw_line(point_3,point_2,r,g,b,a);
	//draw_line(point_3,point_4,r,g,b,a);
	//draw_line(point_2,point_4,r,g,b,a);
}



void Display::draw_triangle_filled(Vector point_min, Vector point_mid, Vector point_max, int r, int g, int b, int a)
{
	// sort based on y
	if(point_min.y > point_mid.y) std::swap(point_min,point_mid);
	if(point_min.y > point_max.y) std::swap(point_min,point_max);
	if(point_mid.y > point_max.y) std::swap(point_mid,point_max);

	float triangle_area = Vector::signed_triangle_area( (point_max - point_min) , (point_mid - point_min));
	int handedness = 0;
	triangle_area>=0?handedness = 1: handedness = 0;
	scan_buffer_line(point_max,point_mid,1-handedness);
	scan_buffer_line(point_mid,point_min,1-handedness);
	scan_buffer_line(point_max,point_min,  handedness);


	draw_scan_buffer( std::ceil(point_min.y),std::ceil(point_max.y),r,g,b,a);


}


void Display::scan_buffer_line(Vector point_max, Vector point_min, int handedness)
{
	if(point_max.y == point_min.y) return;
	float inverse_slope = (point_max.x-point_min.x)/(float)(point_max.y-point_min.y);
	int y_start = std::ceil(point_max.y);
	int y_end = std::ceil(point_min.y);
	float x_ = point_max.x;
	for(; y_start>y_end;y_start--,x_-=inverse_slope)
	{
		//std::cout<<handedness<<" "<<y_start<<std::endl;
		scan_buffer[y_start][handedness] = x_;
	}

}


float Display::get_x_on_line_at_y(Vector point_1,float inverse_slope,float y)
{
	return ( y - point_1.y) * inverse_slope + point_1.x;
}


void Display::draw_scan_buffer(int min_y, int max_y,int r, int g, int b, int a)
{
	//std::cout<<"jjj"<<std::endl;

	for(int i = max_y ; i > min_y ; i--)
		for( int j = scan_buffer[i][0]; j < scan_buffer[i][1] ; j++)
		{
			draw_point(j,i,r,g,b,a);
		}
}


void Display::switch_to_octant_zero_from(int octant, Vector* point)
{
	switch(octant)
	{
	case 0:
		break;
	case 1:
		std::swap( point->x, point->y);
		break;
	case 2:
		point->x = -point->x;
		std::swap(point->x,point->y);
		break;
	case 3:
		point->x = -point->x;
		break;
	case 4:
		point->x = -point->x;
		point->y = -point->y;
		break;
	case 5:
		point->x = -point->x;
		point->y = -point->y;
		std::swap(point->x,point->y);
		break;
	case 6:
		point->y = -point->y;
		std::swap(point->x, point->y);
		break;
	case 7:
		point->y = -point->y;
		break;
	}
}



void Display::switch_from_octant_zero_to(int octant, Vector* point)
{
		switch(octant)
	{
	case 0:
		break;
	case 1:
		std::swap( point->x, point->y);
		break;
	case 2:
		point->y = -point->y;
		std::swap(point->x,point->y);
		break;
	case 3:
		point->x = -point->x;
		break;
	case 4:
		point->x = -point->x;
		point->y = -point->y;
		break;
	case 5:
		point->x = -point->x;
		point->y = -point->y;
		std::swap(point->x,point->y);
		break;
	case 6:
		point->x = -point->x;
		std::swap(point->x, point->y);
		break;
	case 7:
		point->y = -point->y;
		break;
	}
}
