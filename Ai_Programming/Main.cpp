#include <SDL.h>					//including the SDL header file
#include <iostream>					//allows use of the iosteam libary
#include <vector>					//allows for the us of vectors data struct
#include <fstream>					//be able to read text files
#include "Node.h"					//includes my node header file to allows nodes to inherit from their class
#include "Image.h"					//head file for loading images in sdl2
#include <windows.h>
#include <algorithm>
#include "Sprite.h"
#include "Player.h"
#include "State.h"
#include "StateMananger.h"
#include "IdleState.h"

// Function prototypes
void GameLoop(SDL_Window* window, SDL_Renderer* renderer, int _window_w, int _window_h);										 //protocall for gameLoop function
bool MainMenu(SDL_Window* window, SDL_Renderer* renderer);																		//protocall for main menu
void levelBuilder(int const mapHeight, int const mapWidth, int offset, SDL_Renderer* renderer, std::vector<Node*>& blocks);		 //protocall for levelbuilder function
void Instructions(SDL_Window* window, SDL_Renderer* renderer);
Node* breadthFirst(Node* _Start, Node* _endNode, bool &_pathfound, std::vector<Node*>& blocks);									//protocall for node vector function
Node* bestFirst(Node* _Start, Node* _endNode, bool &_pathfound, std::vector<Node*>& blocks);
//Node* A_Star_Attempt(Node* _Start, Node* _endNode, bool &_pathfound, std::vector<Node*>& blocks, int const mapHeight, int const mapWidth);

struct SortF
{
	bool operator() (Node* _node, Node* _node2) { return (_node->getF() < _node2->getF()); }
}
fVal;


int mouseX;    //Gloabal MouseX and Y variables
int mouseY;
SDL_Renderer* renderer;
SDL_Texture * openTex;
SDL_Texture* openTex2;
SDL_Rect position;

StateManager manager;


int main(int, char**)
{
	SDL_Window* window;

	int _window_w = 1200;
	int _window_h = 800;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)   //Try to initialising SDL, log error and pause if fail
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << "\n";
		system("pause");
		return 0;
	}

	window = SDL_CreateWindow("A_Star_Pathfinding", 100, 100, _window_w, _window_h, SDL_WINDOW_SHOWN);   //Try to create the window, log error and pause if fail
	if (window == NULL)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		system("pause");
		return 0;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);   //Try to create the renderer, log error and pause if fail
	if (renderer == NULL)
	{
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return false;
	}

	GameLoop(window, renderer, _window_w, _window_h);								 //This function call goes into the actual game loop

	SDL_DestroyRenderer(renderer);													 //Clean up, free any memeory we have used
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

void GameLoop(SDL_Window* window, SDL_Renderer* renderer, int _window_w, int _window_h)
{
	SDL_Surface* wallBmp = SDL_LoadBMP("Wall.bmp");						//Loads the image to RAM
	SDL_Surface* boxBmp = SDL_LoadBMP("cubeSmall.bmp");
	SDL_Surface* redBmp = SDL_LoadBMP("Red.bmp");
	SDL_Surface* greenBmp = SDL_LoadBMP("Green.bmp");
	SDL_Surface* walkBmp = SDL_LoadBMP("test.bmp");
	SDL_Surface* openBmp = SDL_LoadBMP("Green.bmp");
	SDL_Surface* openBmp2 = SDL_LoadBMP("Orange.bmp");
	SDL_Surface* orangeBmp = SDL_LoadBMP("Orange.bmp");
	SDL_Surface* buttonsBmp = SDL_LoadBMP("buttons.bmp");
	SDL_Texture* wallTex = SDL_CreateTextureFromSurface(renderer, wallBmp);			//Copy image form RAM to GPU
	SDL_Texture* boxTex = SDL_CreateTextureFromSurface(renderer, boxBmp);
	SDL_Texture* redTex = SDL_CreateTextureFromSurface(renderer, redBmp);
	SDL_Texture* greenTex = SDL_CreateTextureFromSurface(renderer, greenBmp);
	SDL_Texture* walkTex = SDL_CreateTextureFromSurface(renderer, walkBmp);
	SDL_Texture* orangeTex = SDL_CreateTextureFromSurface(renderer, orangeBmp);
	SDL_Texture* buttonTex = SDL_CreateTextureFromSurface(renderer, buttonsBmp);

	Sprite* buttons = new Sprite(renderer, "buttons.bmp", 0, 0, 1200, 800);
	Player* ship = new Player(renderer, "Green.bmp", 0, 0);
	bool quit = false;						//while statement that runs the game loop
	SDL_Event e;							//sets sdl events to e for future referance																																																								
	std::vector<Node*> blocks;				//initialise my node 
	int const mapWidth = 24;				//initialise my map width that will be used to draw the grid
	int const mapHeight = 16;				//initialise my map height that will be used to draw the grid
	int offset = 50;						//initialise my offset for the pixel space between my blocks
	int Green = 0;							//Variables used for setting the start and end goal of my path
	int Red = 0;							//Variables used for setting the start and end goal of my path
	int Orange = 0;

	//Node* block = new Node(renderer, "Ayy.bmp", mouseX * offset, mouseY * offset, 50, 50);

	openTex = SDL_CreateTextureFromSurface(renderer, openBmp);
	openTex2 = SDL_CreateTextureFromSurface(renderer, openBmp2);

	SDL_FreeSurface(openBmp);
	SDL_FreeSurface(wallBmp);
	SDL_FreeSurface(redBmp);				//Removing the memory space on RAM
	SDL_FreeSurface(greenBmp);
	SDL_FreeSurface(orangeBmp);
	SDL_FreeSurface(buttonsBmp);
	SDL_FreeSurface(boxBmp);

	MainMenu(window, renderer);
	levelBuilder(mapHeight, mapWidth, offset, renderer, blocks);			//function call for my level builder

	bool haveDoneSearch = false;											//initialise my search boolean

	SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);

	while (!quit)															// This is the main game loop
	{

		SDL_GetMouseState(&mouseX, &mouseY);
		const Uint8* key = SDL_GetKeyboardState(NULL);
		//mouseX = mouseX / 50;
		//mouseY = mouseY / 50;
		//std::cout << mouseX << " " << mouseY << std::endl;



		while (SDL_PollEvent(&e))											//Allow closing SDL window to quit
		{
			if (e.type == SDL_QUIT)											//when the sdl event has quit set quit to true
				quit = true;


			if (e.type == SDL_MOUSEBUTTONDOWN)								//Creating an event type of any sdl mouse button, allowing use of the mouse
			{
				switch (e.button.button)
				{
				case SDL_BUTTON_LEFT:										//What key is needed to be pressed in order for the action to happen
					for (int i = 0; i < blocks.size(); i++)					//For loop that runs through every block that is being created in the blocks vector
					{
						std::cout << blocks[i]->getRect().x << std::endl;	//Outputs the block location to the command window
						if (mouseX >= blocks[i]->getRect().x && mouseX <= blocks[i]->getRect().x + 50 && mouseY >= blocks[i]->getRect().y && mouseY <= blocks[i]->getRect().y + 50) //Allowing for mouse clicks to fit perfectly in the
						{																																							//the squares on the screen with the correct offset	
							std::cout << "press key" << std::endl;									//Destorys the imagine of the block at position i (the current position) in the vector
							ship->setPosition(blocks[i]->getRect());											//Renders the image in that current location (Drawing the new image)
							Red = i;																//Int green is set to the location in the vector of that current block
							/*if (e.type == SDL_KEYDOWN)
							{
								switch (e.button.button)
								{
								case SDL_SCANCODE_LEFT:
									position.x -= 5;

								}

							}*/
							break;																	//This is then set as the start location for my path finding	

						}


					}


				};		//Mouse button pressed

			}
			if (e.type == SDL_MOUSEBUTTONDOWN)											//Creating an event type of any sdl mouse button, allowing use of the mouse
			{
				switch (e.button.button)
				{
				case SDL_BUTTON_RIGHT:
					for (int i = 0; i < blocks.size(); i++)								//Repeated code from above
					{																	//Runs the same function for a different image and on a different mouse input
						std::cout << blocks[i]->getRect().x << std::endl;
						if (mouseX >= blocks[i]->getRect().x && mouseX <= blocks[i]->getRect().x + 50 && mouseY >= blocks[i]->getRect().y && mouseY <= blocks[i]->getRect().y + 50)
						{
							std::cout << "press key" << std::endl;
							blocks[i]->image = redTex;
							haveDoneSearch = false;										//Sets this node to be false meaning it is the cube to be searched for, it now knows it location in the vector
							Green = i;													//Int Red is set to the location in the vector of that current block
							manager.setState(new IdleState(&manager));
							break;														//This is then set as the end goal of my path finding 

						}


					}


				};//mouse button pressed
			

			}
			if (e.type == SDL_KEYDOWN)											//Creating an event type of any sdl mouse button, allowing use of the mouse
			{
				switch (e.button.button)
				{
				case SDL_SCANCODE_W:
					for (int i = 0; i < blocks.size(); i++)								//Repeated code from above
					{																	//Runs the same function for a different image and on a different mouse input
						std::cout << blocks[i]->getRect().x << std::endl;
						if (mouseX >= blocks[i]->getRect().x && mouseX <= blocks[i]->getRect().x + 50 && mouseY >= blocks[i]->getRect().y && mouseY <= blocks[i]->getRect().y + 50)
						{
							std::cout << "press key" << std::endl;
							blocks[i]->image = wallTex;
							blocks[i]->setWalkable(false);					//These blocks are wall and are set an non walkable for the path, it cannot take a path on these blocks	
							Orange = i;
							break;

						}
					}
				};
			}
		}

		if (!haveDoneSearch || ship->moved)												//starting the loop for if the search hasn't been done
		{
			bool pathFound = false;
	
			if (e.type == SDL_MOUSEBUTTONDOWN || ship->moved)							//Creating an event type of any sdl mouse button, allowing use of the mouse
			{
				
				if (ship->moved)
				{
					e.button.button = SDL_BUTTON_MIDDLE;
				}
			
				switch (e.button.button)
				{
				case SDL_BUTTON_MIDDLE:												//SDL MOUSE BUTTON DOWN THING
					if ((mouseX > 1010) && (mouseX < 1190) && (mouseY > 5) && (mouseY < 60) ||
						(ship->moved))
					{
						int pp = Green;
						
						for (int i = 0; i < blocks.size(); i++)
						{
							if (ship->GetX() >= blocks[i]->getRect().x && ship->GetX() <= blocks[i]->getRect().x + 50 && ship->GetY() >= blocks[i]->getRect().y && ship->GetY() <= blocks[i]->getRect().y + 50)
							{
								pp = i;
							
								break;
							}
						}

						Node* goal = bestFirst(blocks.at(Green), blocks.at(pp), pathFound, blocks);	//Starts the breathFirst search when the green and red mouse blocks have been places in the graphics window
						haveDoneSearch = true;													        //once all the nodes have been searched set the search to true
						Node* nextNode = goal;										    //creates a pointer that states that the next node is the goal
						std::vector<Node*> path;									    //creates a vector to pass the path information into, allowing the path to be saved
						while (true)
						{
							path.push_back(nextNode);
							nextNode = nextNode->getParent();                           //the next node will be set as the goal and pushed into the parent node pointer

							if (nextNode == NULL)										//if the goal node isn't a null pointer
							{
								break;								//push that node into the path vector
							}
							
																					//if not end
						}
						for (Node* n : blocks)
						{
							if (n->image == redTex)
							{

								n->image = boxTex; //Change path image back to blocks

							}
						
						}

						for (Node* n : path)
						{

							n->image = redTex;			//drawing path

							

							for (Node* block : blocks)										//for every Node* in the blocks vector
							{
								//block->setSearch(false);
								block->Draw();												//call it's draw function
							}

							//SDL_RenderPresent(renderer);
							//Sleep(100);
						}
					}
				}
				
				//SDL_RenderClear(renderer);
				SDL_RenderPresent(renderer);
				
				ship->moved = false;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN || ship->moved)											//Creating an event type of any sdl mouse button, allowing use of the mouse
			{
				if (ship->moved)
				{
					e.button.button = SDL_BUTTON_MIDDLE;
				}
				switch (e.button.button)
				{
				case SDL_BUTTON_MIDDLE:
					if ((mouseX > 1010) && (mouseX < 1190) && (mouseY > 69) && (mouseY < 125) ||
						(ship->moved))
					{
						int pp = Green;
						for (int i = 0; i < blocks.size(); i++)
						{
							if (ship->GetX() >= blocks[i]->getRect().x && ship->GetX() <= blocks[i]->getRect().x + 50 && ship->GetY() >= blocks[i]->getRect().y && ship->GetY() <= blocks[i]->getRect().y + 50)
							{
								pp = i;
								break;
							}
						}
						//Node* goal = A_Star_Attempt(blocks.at(Green), blocks.at(Red), pathFound, blocks, mapHeight, mapWidth);
						Node* goal = breadthFirst(blocks.at(Green), blocks.at(pp), pathFound, blocks);	//Starts the breathFirst search when the green and red mouse blocks have been places in the graphics window
						haveDoneSearch = true;													        //once all the nodes have been searched set the search to true
						Node* nextNode = goal;										    //creates a pointer that states that the next node is the goal
						std::vector<Node*> path;									    //creates a vector to pass the path information into, allowing the path to be saved
						while (true)
						{
							path.push_back(nextNode);
							nextNode = nextNode->getParent();                        //the next node will be set as the goal and pushed into the parent node pointer

							if (nextNode == NULL)										//if the goal node isn't a null pointer
							{
								break;								//push that node into the path vector
							}
							
						}

						for (Node* n : blocks)
						{
							if (n->image == redTex)
							{

								n->image = boxTex; //Change path image back to blocks

							}

						}

						for (Node* n : path)
						{

							n->image = redTex;			//drawing path



							for (Node* block : blocks)										//for every Node* in the blocks vector
							{
								//block->setSearch(false);
								block->Draw();												//call it's draw function
							}

							//SDL_RenderPresent(renderer);
							//Sleep(100);
						}
					}
				}

				//SDL_RenderClear(renderer);
				SDL_RenderPresent(renderer);

				ship->moved = false;
			}
		}

		SDL_RenderClear(renderer);										//First clear the renderer

		for (Node* block : blocks)										//for every Node* in the blocks vector
		{
			block->Draw();												//call it's draw function
			
		}
		
		
		
		ship->Update();
		ship->Draw();
		buttons->Draw();
		SDL_RenderPresent(renderer);									//Update the screen

		if (e.type == SDL_MOUSEBUTTONDOWN)											//Creating an event type of any sdl mouse button, allowing use of the mouse
		{
			switch (e.button.button)
			{
			case SDL_BUTTON_MIDDLE:												//SDL MOUSE BUTTON DOWN THING
				if ((mouseX > 1033) && (mouseX < 1165) && (mouseY > 140) && (mouseY < 170))
				{
					blocks.clear();
					levelBuilder(mapHeight, mapWidth, offset, renderer, blocks);
					SDL_RenderClear(renderer);
					break;
				}
			}
		}
		manager.update();
	}

	
	
	
																		
	for (Node* block : blocks)                                          //game loop ended, free all the memory used by the Node objects
		delete block;

	blocks.clear();														//empty the vector

	SDL_DestroyTexture(wallTex);
	SDL_DestroyTexture(redTex);
	SDL_DestroyTexture(greenTex);
	SDL_DestroyTexture(walkTex);
	SDL_DestroyTexture(openTex);
}

bool MainMenu(SDL_Window* window, SDL_Renderer* renderer)
{

	bool menu = true;																			//On start the menu is set to true


	Sprite* main_menu = new Sprite(renderer, "Main_Menu.bmp", 0, 0, 1200, 800);					//The sprite is initialised at the size of the screen

	int MouseX = { 0 };																			//Mouse x and y position is created
	int MouseY = { 0 };

	while (menu)																				// Run while isPlaying variable is set to true
	{
		SDL_PumpEvents();																		//Allows mouse inputs

		int x = SDL_GetMouseState(&MouseX, &MouseY) & 1;										//Take possition of mouse x and y
		std::cout << MouseX << " " << MouseY << std::endl;

		if (x == 1)    // If left mouse button was pressed
		{
			if ((MouseX > 520) && (MouseX < 690) && (MouseY > 286) && (MouseY < 360))			//Sets the parameters for the Play button on my menu
			{
				menu = false;
				return true;
				
			}

			if ((MouseX > 490) && (MouseX < 720) && (MouseY > 385) && (MouseY < 435))			//Sets the parameters for the instructions button on my menu
			{
				Instructions(window, renderer);													//Calls the instructions function
			}

			if ((MouseX > 595) && (MouseX < 605) && (MouseY > 481) && (MouseY < 510))			//Sets the parameters for the quit button on my menu
			{
				delete main_menu;																  // Deletes the main menu object
				SDL_Quit();																		 // Closes the SDL graphics window
				exit(0);																		// Closes the command console
			}
		}

		SDL_RenderClear(renderer);

		main_menu->Draw();																	   // Draw the main menu bmp

		SDL_RenderPresent(renderer);
	}
}

void Instructions(SDL_Window* window, SDL_Renderer* renderer)
{
	bool Instructions = true;																		//On start the Instructions is set to true
	int MouseX = { 0 };																				//Mouse x and y position is created
	int MouseY = { 0 };
	int x;
	
	Sprite* instructionsPage = new Sprite(renderer, "Instructions.bmp", 0, 0, 1200, 800);		//The sprite is initialised at the size of the screen

	while (Instructions == true)
	{
		SDL_PumpEvents();
		std::cout << MouseX << " " << MouseY << std::endl;
		x = SDL_GetMouseState(&MouseX, &MouseY) & 1;
		if (x == 1)																					 // If left mouse button was pressed
		{
			if ((MouseX > 520) && (MouseX < 680) && (MouseY > 672) && (MouseY < 705))				 //Sets the parameters for the back button on my window
			{
				Instructions = false;

				delete instructionsPage;															 // Deletes the Instruction page object
				return;
			}
		}

		SDL_RenderClear(renderer);

		instructionsPage->Draw();																	// Draw the instructions bmp

		SDL_RenderPresent(renderer);
	}
}



void levelBuilder(int const mapHeight, int const mapWidth, int offset, SDL_Renderer* renderer, std::vector<Node*>& blocks)
{
	Sprite* button = new Sprite(renderer, "buttons.bmp", 0, 0, 1200, 800);

	SDL_RenderClear(renderer);

	std::ifstream inFile("map.txt");
	if (inFile.is_open())
	{
		for (int y = 0; y < mapHeight; y++)								//start the first of two for loops to run across the y axis of the grid
		{																
			for (int x = 0; x < mapWidth; x++)							//start the second for loop to run across the x axis of the grid
			{															
				
				char c; 
				do
				{
					c = (char)inFile.get();								//opens my text file to read in the integer
				} while (c == '\n');

				//std::cout << c << ',';
				std::cout << "x: " << x << ", y: " << y << "\n";		//out puts the xy positions to the command window

				Node* block = new Node();								//make new Node object

				switch (c)												//starts a new switch statement for the reading of the text file for my grid
				{
				case '0':
					block = new Node(renderer, "cubeSmall.bmp", x, y, 50, 50, offset);   //if the number read is 0 then draw the new node with the name cubeSmall.bmp in my debug folder
					break;																		   //draw at the current position of the x & y for loop and mutilply it by the offset, size 50 / 50 pixels
				case '1':
					block = new Node(renderer, "Breadth.bmp", x, y, 50, 50, offset);       //if the number read is 1 then draw the new node with the name Green.bmp in my debug folder
																								   //draw at the current position of the x & y for loop and mutilply it by the offset, size 50 / 50 pixels
					break;
				case '2':
					block = new Node(renderer, "BestFirst.bmp", x, y, 50, 50, offset);		  //if the number read is 2 then draw the new node with the name Red.bmp in my debug folder
																								  //draw at the current position of the x & y for loop and mutilply it by the offset, size 50 / 50 pixels
					break;
				case '3':
					block = new Node(renderer, "Ayy.bmp", x, y, 50, 50, offset);        //if the number read is 3 then draw the new node with the name Ayy.bmp in my debug folder
																								  //draw at the current position of the x & y for loop and mutilply it by the offset, size 50 / 50 pixels
					break;
				case '4':
					block = new Node(renderer, "wall.bmp", x, y, 50, 50, offset);       //if the number read is 3 then draw the new node with the name Ayy.bmp in my debug folder
					block->setWalkable(false);													  //block 4 is set to non walkable for areas my agent can not walk
					break;																		  //draw at the current position of the x & y for loop and mutilply it by the offset, size 50 / 50 pixels
				default:
					std::cout << "Unexpected character in map file: " << c << "\n";				  //if any errors come up out put to the command window that there has been an error
					break;
				}
				blocks.push_back(block);														  //store the blocks in this vector so we can access them easilly
			}
		}
	}

	inFile.close();													//close text file


	int i = 0;
	for (int y = 0; y < mapHeight; y++) {
		
		for (int x = 0; x < mapWidth; x++) {

			if (x == 0)													//if the current block is in the top left of the screen
			{
				// east
				blocks.at(i)->setNeighbour(blocks.at(i + 1));			//The current block will be set into the neighbours vector
																		//The current block will then move one step east and add this block to the same vector
			}
			else if (x == mapWidth - 1)									//If the blocks x position is equal to the map width then take one as the block is at the egde
			{															//of the screen
				// west
				blocks.at(i)->setNeighbour(blocks.at(i - 1));			//The blocks will then moved down one and move west

			}
			else
			{
				// add them if valid
				blocks.at(i)->setNeighbour(blocks.at(i + 1));			//If the blocks can move in either direction, as they are not at the screen left or right
				blocks.at(i)->setNeighbour(blocks.at(i - 1));			//The had nodes either side so add both
			}
			if (y == 0)
			{

				blocks.at(i)->setNeighbour(blocks.at(i + mapWidth));	//adding neighbouring nodes in the y axis in the same way the blocks where added before
			}															//Starting with the y axis in the top left
			else if (y == mapHeight - 1)
			{
				blocks.at(i)->setNeighbour(blocks.at(i - mapWidth));	//When the y axis reaches the bottom left move across to the right

			}
			else
			{
				blocks.at(i)->setNeighbour(blocks.at(i + mapWidth));	//If the blocks can move either way they are in the middle
				blocks.at(i)->setNeighbour(blocks.at(i - mapWidth));	//So do both
			}
			i++;
		}

	}
}

Node* breadthFirst(Node* _Start, Node* _endNode, bool &_pathfound, std::vector<Node*>& blocks)
{

	std::vector < Node*> Open;					//Creates a new vector called Open

	for (size_t i = 0; i < blocks.size(); i++)
	{
		blocks.at(i)->setParent(NULL);
		blocks.at(i)->setClosed(false);
	}

	Open.clear();								//Clears that vector
	std::vector <Node*> Neighbours;				//Creates Neighbour vector
	Open.push_back(_Start);						//Opens the vector at the start

	while (Open.size() > 0/*_pathfound != true*/)	
	{											//and Neighbours vector as well as being set to true from the searched bool
		if (Open.front() == _endNode)			//If the first item in the open vector is the end node
		{
			_pathfound = true;					//The the path has been found, path found will be set to true, this will then skip to clear
			break;
		}										//the Neighbours and return the Open vector
		else
		{
			Neighbours = Open.front()->getneighbour();      //If the first node isn't the end node then push that node into the neighbours vector
			for (size_t i = 0; i < Neighbours.size(); i++)	//For loop to run through the whole Neighbours vector
			{
				bool found = false;							//Sets each node in the Neighbours vector to false
				for (size_t o = 0; o < Open.size(); o++)	//Second for loop to run through the whole Open vector
				{
					if (Open.at(o) == Neighbours.at(i))     //If the current node in Open is equal to the same node in Neighbours
					{
						found = true;						//Found is changed to true
						break;
					}
				}

				if (found) continue;						//If found is true continue

				if (!Neighbours.at(i)->getSearched() && Neighbours.at(i)->getWalkable())		//If that current node in Neighbours is in searched and that
				{																				//that current node is walkable
					Neighbours.at(i)->setParent(Open.front());									//Set that current node to the Parent
					Open.push_back(Neighbours.at(i));											//Push that Neighbour to the bottom of the stack
					//Neighbours.at(i)->image = openTex;
				}

			}
			Open.front()->setSearch(true);					//When it reaches here all the nodes have been checked so it will open at the front of Open and set the search to true
			Open.erase(Open.begin());						//Erase destroys the elements of the vector, begin returns an iterator pointing to the first element in the vector
		}

		
		/*SDL_RenderClear(renderer);

		for (Node* block : blocks)
		{
			block->Draw();
		}

		SDL_RenderPresent(renderer);
		Sleep(10);*/
		
	}
	Neighbours.clear();									 //Removes all elements from the vector (which are destroyed), leaving the container with a size of 0.

	if (Open.size() < 1)
	{
		return _endNode;
	}
	return Open.front();								//The returns at the front of Open
}

Node* bestFirst(Node* _Start, Node* _endNode, bool &_pathfound, std::vector<Node*>& blocks)
{

	std::vector < Node*> Open;					//Creates a new vector called Open
	std::vector < Node*> Closed;

	for (size_t i = 0; i < blocks.size(); i++)
	{
		blocks.at(i)->setParent(NULL);
		blocks.at(i)->setClosed(false);
	}

	Open.clear();								//Clears that vector
	std::vector <Node*> Neighbours;				//Creates Neighbour vector
	Open.push_back(_Start);						//Opens the vector at the start

	while (Open.size() > 0/*_pathfound != true*/)					//Main loop that run while my neighours at being passed into the Open [***************]: add the statement (Open vector is NOT empty) in your condition statement [***************]
	{											//and Neighbours vector as well as being set to true from the searched bool
		Node* cbest = Open.front();

		for (size_t i = 0; i < Open.size(); i++)
		{
			if (cbest->distanceFromGoal(_endNode) > Open.at(i)->distanceFromGoal(_endNode))
			{
				cbest = Open.at(i);
			}
		}


		if (Open.front() == _endNode)			//If the first item in the open vector is the end node
		{
			_pathfound = true;					//The the path has been found, path found will be set to true, this will then skip to clear
			break;
		}										//the Neighbours and return the Open vector
		else
		{
			Neighbours = Open.front()->getneighbour();	//If the first node isn't the end node then push that node into the neighbours vector
			for (size_t i = 0; i < Neighbours.size(); i++)	//For loop to run through the whole Neighbours vector
			{
				bool found = false;							//Sets each node in the Neighbours vector to false
				for (size_t c = 0; c < Open.size(); c++)	//Second for loop to run through the whole Open vector
				{
					if (Neighbours.at(i) == Open.at(c))			//If the current node in Open is equal to the same node in Neighbours
					{
						found = true;						//Found is changed to true
						break;
					}
				}
				for (size_t c = 0; c < Closed.size(); c++)	//Second for loop to run through the whole Open vector
				{
					if (Neighbours.at(i) == Closed.at(c))			//If the current node in Open is equal to the same node in Neighbours
					{
						found = true;						//Found is changed to true
						break;
					}
				}

				if (found) continue;						//If found is true continuse

				if (!Neighbours.at(i)->getClosed() //Should be the closed vector
					&& Neighbours.at(i)->getWalkable())		//If that current node in Neighbours is in CLOSED and that
				{																				//that current node is walkable
					Neighbours.at(i)->setParent(Open.front());									//Set that current node to the Parent
					int xAway = abs(Neighbours[i]->getx() - _endNode->getx());					//calculate the distance from the x of the neighbour to the x of the end node
					int yAway = abs(Neighbours[i]->gety() - _endNode->gety());					//calculate the distance from the y of the neighbour to the y of the end node
					Neighbours[i]->setH(xAway + yAway);											//add them two values together to give you how many x and y steps are needed
					Neighbours[i]->setF();
					//Neighbours.at(i)->image = openTex2;											//used to draw a visual of the search

					Open.push_back(Neighbours.at(i));											//Push that Neighbour to the bottom of the stack
																								//Neighbours.at(i)->image = openTex;
				}

			}
			//Open.front()->setSearch(true);						//When it reaches here all the nodes have been checked so it will open at the front of Open and set the search to true
			Closed.push_back(Open.front());
			Open.erase(Open.begin());								//Erase destroys the elements of the vector, begin returns an iterator pointing to the first element in the vector
			if(!Open.empty())
			{ 
				std::sort(Open.begin(), Open.end(), fVal);			// sorting the Open vector according to the fVal criterion
			}

		}

		
		/*SDL_RenderClear(renderer);

		for (Node* block : blocks)
		{
		block->Draw();
		}

		SDL_RenderPresent(renderer);
		Sleep(10);*/
		
	}
	Neighbours.clear();									 //Removes all elements from the vector (which are destroyed), leaving the container with a size of 0.
	if (Open.size() < 1)
	{
		return _endNode;
	}
	return Open.front();								//The returns at the front of Open
}

//Node* A_Star_Attempt(Node* _Start, Node* _endNode, bool &_pathfound, std::vector<Node*>& blocks, int const mapHeight, int const mapWidth)
//{
//
//	std::vector < Node*> Open;					//Creates a new vector called Open
//	std::vector < Node*> Closed;
//
//	for (size_t i = 0; i < blocks.size(); i++)
//	{
//		blocks.at(i)->setParent(NULL);
//		blocks.at(i)->setClosed(false);
//	}
//
//	Open.clear();								//Clears that vector
//	std::vector <Node*> Neighbours;				//Creates Neighbour vector
//	Open.push_back(_Start);						//Opens the vector at the start
//
//	while (Open.size() > 0/*_pathfound != true*/)					//Main loop that run while my neighours at being passed into the Open [***************]: add the statement (Open vector is NOT empty) in your condition statement [***************]
//	{											//and Neighbours vector as well as being set to true from the searched bool
//		Node* cbest = Open.front();
//
//		for (size_t i = 0; i < Open.size(); i++)
//		{
//			if (cbest->distanceFromGoal(_endNode) > Open.at(i)->distanceFromGoal(_endNode))
//			{
//				cbest = Open.at(i);
//			}
//		}
//
//
//		if (Open.front() == _endNode)			//If the first item in the open vector is the end node
//		{
//			_pathfound = true;					//The the path has been found, path found will be set to true, this will then skip to clear
//			break;
//		}										//the Neighbours and return the Open vector
//		else
//		{
//			Neighbours = Open.front()->getneighbour();	//If the first node isn't the end node then push that node into the neighbours vector
//			for (size_t i = 0; i < Neighbours.size(); i++)	//For loop to run through the whole Neighbours vector
//			{
//				bool found = false;							//Sets each node in the Neighbours vector to false
//				for (size_t c = 0; c < Open.size(); c++)	//Second for loop to run through the whole Open vector
//				{
//					if (Neighbours.at(i) == Open.at(c))			//If the current node in Open is equal to the same node in Neighbours
//					{
//						found = true;						//Found is changed to true
//						break;
//					}
//				}
//				for (size_t c = 0; c < Closed.size(); c++)	//Second for loop to run through the whole Open vector
//				{
//					if (Neighbours.at(i) == Closed.at(c))			//If the current node in Open is equal to the same node in Neighbours
//					{
//						found = true;						//Found is changed to true
//						break;
//					}
//				}
//
//				if (found) continue;						//If found is true continuse
//
//				if (!Neighbours.at(i)->getClosed() //Should be the closed vector
//					&& Neighbours.at(i)->getWalkable())		//If that current node in Neighbours is in CLOSED and that
//				{																				//that current node is walkable
//					Neighbours.at(i)->setParent(Open.front());									//Set that current node to the Parent
//					int xAway = abs(Neighbours[i]->getx() - _endNode->getx());					//calculate the distance from the x of the neighbour to the x of the end node
//					int yAway = abs(Neighbours[i]->gety() - _endNode->gety());					//calculate the distance from the y of the neighbour to the y of the end node
//					Neighbours[i]->setH(xAway + yAway);											//add them two values together to give you how many x and y steps are needed
//					Neighbours[i]->setFG();
//					Neighbours.at(i)->image = openTex2;											//used to draw a visual of the search
//
//					Open.push_back(Neighbours.at(i));											//Push that Neighbour to the bottom of the stack
//																								//Neighbours.at(i)->image = openTex;
//				}
//
//			}
//			Closed.push_back(Open.front());
//			Open.erase(Open.begin());								//Erase destroys the elements of the vector, begin returns an iterator pointing to the first element in the vector
//			if (!Open.empty())
//			{
//				std::sort(Open.begin(), Open.end(), fVal);			// sorting the Open vector according to the fVal criterion
//			}
//
//		}
//
//
//		SDL_RenderClear(renderer);
//
//		for (Node* block : blocks)
//		{
//			block->Draw();
//		}
//
//		SDL_RenderPresent(renderer);
//		Sleep(10);
//
//	}
//	Neighbours.clear();									 //Removes all elements from the vector (which are destroyed), leaving the container with a size of 0.
//	if (Open.size() < 1)
//	{
//		return _endNode;
//	}
//	return Open.front();								//The returns at the front of Open
//}
