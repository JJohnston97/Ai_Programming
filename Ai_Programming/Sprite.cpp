#include "Sprite.h"																			//Includes sprite

Sprite::Sprite(SDL_Renderer* _renderer, char* _file, int _x, int _y, int _w, int _h)		//Consutor of the class
{
	SDL_Surface* bmpFile = SDL_LoadBMP(_file);						//Loads int he bmp file
	image = SDL_CreateTextureFromSurface(_renderer, bmpFile);		//Render the image to the window
	SDL_FreeSurface(bmpFile);

	position.x = _x;												//Initial Position & width and height of the sprite
	position.y = _y;												//Based on incoming parameters
	position.w = _w;
	position.h = _h;

	renderer = _renderer;

}

Sprite::~Sprite()													//Destructor function is being called to destory a sprite
{

	if (image)
		SDL_DestroyTexture(image);									//Destory Sprite


}


void Sprite::Draw()													//Draws the sprite image to the screen
{

	if (image)
		SDL_RenderCopy(renderer, image, NULL, &position);


}