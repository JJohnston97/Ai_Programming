#pragma once

#include <SDL.h>														//Includes the sdl libary

class Sprite
{

public:
	Sprite(SDL_Renderer* _renderer, char* _file, int _x, int _y, int _w, int _h);					 //Constructor
	~Sprite();																						//Destructor 


	virtual void Draw();

	void SetX(int _x) { position.x = _x; }								//Asking for the x and y positions for the sprite to be drawn at
	void SetY(int _y) { position.x = _y; }

	int GetX() { return position.x; }
	int GetY() { return position.y; }




protected:

	SDL_Renderer* renderer;				//Sdl Render using for drawing
	SDL_Texture* image;					//Image used by this sprite

	SDL_Rect position;					//x&y position and width, height of this sprite

};