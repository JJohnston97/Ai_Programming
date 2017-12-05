#include "Player.h"

Player::Player(SDL_Renderer* _renderer, char* _file, int _w, int _h) :
	Sprite(_renderer, _file, 100, 100, _w, _h)
{
	timer = 0;
	moved = false;


}

void Player::Update()
{
	timer--;

	if (timer > 0)
	{
		return;
	}

	const Uint8* key = SDL_GetKeyboardState(NULL);

	if (key[SDL_SCANCODE_LEFT])
	{
		position.x -= 50;
		timer = 10;
		moved = true;
	}
	
	if (key[SDL_SCANCODE_RIGHT])
	{
		position.x += 50;
		timer = 10;
		moved = true;
	}
	
	if (key[SDL_SCANCODE_UP])
	{
		position.y -= 50;
		timer = 10;
		moved = true;
	}
	
	if (key[SDL_SCANCODE_DOWN])
	{
		position.y += 50;
		timer = 10;
		moved = true;
	}


}