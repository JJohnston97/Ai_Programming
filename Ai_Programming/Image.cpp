#include "Image.h"

Image::Image(SDL_Renderer* _renderer, char* _file, int _x, int _y, int _w, int _h)
{
	SDL_Surface* bmpFile = SDL_LoadBMP(_file);
	image = SDL_CreateTextureFromSurface(_renderer, bmpFile);
	SDL_FreeSurface(bmpFile);

	position.x = _x;
	position.y = _y;
	position.w = _w;
	position.h = _h;

	renderer = _renderer;
}

Image::~Image()
{
	if (image)
		SDL_DestroyTexture(image);
}

void Image::Draw()
{
	if (image)
		SDL_RenderCopy(renderer, image, NULL, &position);
}