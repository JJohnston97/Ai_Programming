#include "Node.h"

Node::Node() {}
Node::Node(SDL_Renderer* _renderer, char* _file, int _x, int _y, int _w, int _h, int offset)
{
	SDL_Surface* bmpFile = SDL_LoadBMP(_file);
	image = SDL_CreateTextureFromSurface(_renderer, bmpFile);
	SDL_FreeSurface(bmpFile);

	position.x = _x * offset;
	position.y = _y * offset;
	position.w = _w;
	x = _x;
	y = _y;
	position.h = _h;


	renderer = _renderer;

	
	Walkable = true;
	Searched = false;
	Parent = NULL;
}

Node::~Node()
{
	if (image)
		SDL_DestroyTexture(image);
}

void Node::setNeighbour(Node * _node)
{
	Neighbours.push_back(_node);
}

void Node::Draw()
{
	if (image)
	{
		SDL_RenderCopy(renderer, image, NULL, &position);
	}
}

float Node::distanceFromGoal(Node* goal)
{
	int xAway = abs(getx() - goal->getx());
	int yAway = abs(gety() - goal->gety());  //Manhattan Heuristic for working out distance between nodes

	return xAway + yAway;
}