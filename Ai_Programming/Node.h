#pragma once
#include <SDL.h>  //inherits from SDL2
#include <vector> //Uses the Vector libary

class Node
{
public:

	Node();
	Node(SDL_Renderer* _renderer, char* _file, int _x, int _y, int _w, int _h, int offset);   // Constructor
	~Node();																	  // Destructor
	
	
	std::vector<Node*> getneighbour() { return Neighbours; }  //Creates a new vector of getneighbours from the returned Neighbours
	Node* getParent() { return Parent; }					  //Take the location of the Parent node
	bool getSearched() { return Searched; }					  //Takes the returned search, walkable and start
	bool getClosed() { return Closed; }
	bool getWalkable() { return Walkable; }					  //true or false values
	bool getStart() { return Start; }	
	int getG() { return G; }
	int getH() { return H; }
	int getx() { return x; }
	int gety() { return y; }
	int getF() { return F; }
	void SetX(int _x) { position.x = _x; }								//Asking for the x and y positions for the sprite to be drawn at
	void SetY(int _y) { position.x = _y; }
	int GetX() { return position.x; }
	int GetY() { return position.y; }

	void setSearch(bool _b) { Searched = _b; }					//Sets all the values for searched, walkable and neighbour
	void setClosed(bool _b) { Closed = _b; Searched = _b; }
	void setWalkable(bool _b) { Walkable = _b; }
	void setNeighbour(Node* _node);
	bool Walkable;
	SDL_Rect getRect() { return position;  }
	
	virtual void Draw();

	float distanceFromGoal(Node* goal);

	void setParent(Node* _n) { Parent = _n; }
	void setG(int _G) { G = _G; }
	void setH(int _H) { H = _H; }
	void setF() { F = H; }			// WARNING: F is set to the Heuristic only <- this is for BestFirst; it should be F = G + H for A*
	void setFG() { F = (G + H); }

	SDL_Texture* image;				 //A structure that contains an efficient, driver-specific representation of pixel data
	SDL_Rect position;

private:
	
	std::vector<Node*> Neighbours;	//Private Neighbours vector on accessible from within the class
	Node* Parent;

	int F = 0;
	int G = 0;
	int H = 0;
	int x = 0;
	int y = 0;
	
	bool Searched;				//Ture or false value for nodes being searched
	bool Goal;					//The end node
	bool Start;					//The start node
	bool Closed;


protected:					   //Used protected because protected members are accessible in the class and derived classes

	SDL_Renderer* renderer;    // SDL Renderer used for drawing

	//SDL_Rect position;		   //Defines a rectangular area of the screen



};