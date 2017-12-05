#pragma once
#include <SDL.h>   //Inherits from SDL.H

class Image
{
public:

	Image(SDL_Renderer* _renderer, char* _file, int _x, int _y, int _w, int _h);   // Constructor
	~Image();																	   // Destructor

	virtual void Draw();

protected:					//Use protected because protected members are accessible in the class and derived classes

	SDL_Renderer* renderer;   //SDL Renderer used for drawing

	SDL_Texture* image;       //A structure that contains an efficient, driver-specific representation of pixel data

	SDL_Rect position;        //Defines a rectangular area of the screen
};
