#include <iostream> // For std::cout, std::cin
#include <fstream>  // For std::fstream
#include <vector>   // For std::vector
#include <conio.h>  // For _getch()
#include <cassert>  // For assert()
#include <ctime>    // For time()
#include <stdlib.h> // For srand(), rand()
#include <thread> // We run the timer in a different thread so it doesnt alter the gameplay.

#include "ConsoleUtils.h"
#include "TimeUtils.h"

const int MAZE_WIDTH	= 25;
const int MAZE_HEIGHT	= 13;

const int KEY_UP		= 72;
const int KEY_DOWN		= 80;
const int KEY_LEFT		= 75;
const int KEY_RIGHT		= 77;

const int CHEAT_FREEZE	= 70;
const int CHEAT_KILL	= 88;
const int CHEAT_FEED	= 69;

const char EMPTY		= ' ';
const char WALL			= '#';
const char HOLE			= '0';
const char PLAYER		= '@';
const char PILL			= '*';
const char ZOMBIE		= 'Z';

//I have to create a global var for now becuase id have to run this through many functions, I still need to understand the code more.
int MAGICDURATION		= 0;
int TIMER				= 0;
bool FINISHED			= false; //Sorry I made this global, need the other thread to always access it.


WORD getPlayerColour() {
	//im sure we will have more colours later on for the player thats why I created this func, will probs remove if not used later on.

	return MAGICDURATION >= 1 ? clBlue : clWhite;
}

void addMagicEffect() {
	//This class is probs not needed either 
	MAGICDURATION = 10;
}

void updateMagicEffect() {
	if (MAGICDURATION  > 0)
		cout << '\a';
		MAGICDURATION--;
}



// Draws a string to the console at a given position.
void displayMessage(const std::string &msg, int x, int y, int bg, int fg)
{
	GotoXY(x, y);
	SelectBackColour(bg);
	SelectForeColour(fg);
	std::cout << msg;
}

void updateTimer() {
	time_t startSec, lastSec;
	int timePassed;
	bool famDo = true;

	time(&startSec);

	while (famDo)
	{
		time(&lastSec);
		timePassed = difftime(lastSec, startSec);
		if (timePassed == 1)
		{
			TIMER++;
			displayMessage("Timer:  " + std::to_string(TIMER) + "secs", 50, 3, clBlack, clYellow);
			time(&startSec);
		}
		if (FINISHED) {
			famDo = false;
			std::terminate;
		}


	}

}

// A collection a displayMessage calls that produce the game's UI.
void drawUI(
	const std::string &date,
	const std::string &time,
	const std::string &playerName,
	const int playerLives,
	const int playerHighscore,
	const int pillCount)
{
	displayMessage("DATE:  " + date, 50, 4, clBlack, clWhite);
	displayMessage("TIME:  " + time, 50, 5, clBlack, clWhite);

	displayMessage("TO MOVE USE KEYBOARD ARROWS", 50, 7, clBlack, clWhite);
	displayMessage("TO FREEZE ZOMBIES PRESS 'F'", 50, 8, clBlack, clWhite);
	displayMessage("TO KILL ZOMBIES PRESS 'X'", 50, 9, clBlack, clWhite);
	displayMessage("TO QUIT PRESS 'Q'", 50, 10, clBlack, clWhite);

	displayMessage("REMAINING LIVES:   " + std::to_string(playerLives), 50, 14, clBlack, clWhite);
	displayMessage("REMAINING ZOMBIES: ", 50, 15, clBlack, clWhite);
	displayMessage("REMAINING PILLS:   " + std::to_string(pillCount), 50, 16, clBlack, clWhite);

	displayMessage("PLAYER NAME:      " + playerName, 50, 20, clBlack, clWhite);
	displayMessage("PLAYER HIGHSCORE: " + std::to_string(playerHighscore), 50, 21, clBlack, clWhite);
}

// Display information about the game, the current date and time and
// prompts the user to enter their name.
void displayMainMenu(const std::string &date, const std::string &time)
{
	displayMessage("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", 5, 3, clBlack, clWhite);
	displayMessage("| Spot & Zombies: The Basic Edition |", 5, 4, clBlack, clWhite);
	displayMessage("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", 5, 5, clBlack, clWhite);

	displayMessage("GROUP CS# : 2017 - 2018", 5, 10, clBlack, clWhite);
	displayMessage("Thomas Brown        27027319", 5, 11, clBlack, clWhite);
	displayMessage("Killian Comerford   2#######", 5, 12, clBlack, clWhite);
	displayMessage("Callum Beckwith     2#######", 5, 13, clBlack, clWhite);

	displayMessage("DATE:  " + date, 50, 4, clBlack, clWhite);
	displayMessage("TIME:  " + time, 50, 5, clBlack, clWhite);

	displayMessage("TO MOVE USE KEYBOARD ARROWS", 50, 7, clBlack, clWhite);
	displayMessage("TO FREEZE ZOMBIES PRESS 'F'", 50, 8, clBlack, clWhite);
	displayMessage("TO KILL ZOMBIES PRESS 'X'", 50, 9, clBlack, clWhite);
	displayMessage("TO QUIT PRESS 'Q'", 50, 10, clBlack, clWhite);

	displayMessage("Enter your name: ", 5, 20, clBlack, clWhite);
}

// Draws both the player's and zombie's messages to the console.
void drawMessages(
	const std::string &playerMessage,
	const std::string &zombieMessage,
	const std::string &miscMessage)
{
	displayMessage("SPOT " + playerMessage, 0, 2 + MAZE_HEIGHT, clBlack, clWhite);
	displayMessage("ZOMBIE(S) " + zombieMessage, 0, 3 + MAZE_HEIGHT, clBlack, clWhite);
	displayMessage(miscMessage, 0, 4 + MAZE_HEIGHT, clBlack, clWhite);
}

void saveScore(const std::string &name, const int score)
{
	std::string filename = name + ".txt";
	std::ofstream file(filename.c_str());
	file << std::to_string(score);
	file.close();
}

int loadScore(const std::string &name)
{
	int score = -1;

	std::string filename = name + ".txt";
	std::ifstream file(filename.c_str());
	if (file)
	{
		std::string scoreStr;
		getline(file, scoreStr);
		score = std::stoi(scoreStr);
	}

	file.close();
	return (score);
}

int getKeyPress()
{
	int key;
	while ((key = _getch()) == 224);
	return toupper(key);
}

bool isArrowKey(const int key)
{
	return (key == KEY_LEFT) || (key == KEY_RIGHT) || (key == KEY_UP) || (key == KEY_DOWN);
}

bool isCheatKey(const int key)
{
	return (key == CHEAT_FREEZE || key == CHEAT_KILL || key == CHEAT_FEED);
}

struct Object
{
	char symbol;
	int x, y;
	// Spawn coordinate (For zombies).
	int sx, sy;
};

void spawnObject(Object &obj, char grid[][MAZE_WIDTH])
{
	int x, y;
	do
	{
		x = 1 + rand() % (MAZE_WIDTH - 2);
		y = 1 + rand() % (MAZE_HEIGHT - 2);
	} while (grid[y][x] != EMPTY
		|| ((y == 1 || y == MAZE_HEIGHT - 2)
			&& (x == 1 || x == MAZE_WIDTH - 2)));

	// Once a valid position has been found, place this
	// into the grid so it will be taken into account
	// if this function is called again.
	grid[y][x] = obj.symbol;

	// Finally set the objects position.
	obj.x = x;
	obj.y = y;
}

void placeObject(const Object &obj, char grid[][MAZE_WIDTH])
{
	grid[obj.y][obj.x] = obj.symbol;
}

int findObject(std::vector<Object> &objects, int x, int y)
{
	// Find which pill we hit.
	bool found = false;
	unsigned index = 0;
	while (!found && index < objects.size())
	{
		if (objects.at(index).x == x &&
			objects.at(index).y == y)
		{
			found = true;
			break;
		}
		index++;
	}

	if (!found)
		index = -1;

	return index;
}

void moveObject(Object &obj, int dx, int dy)
{
	obj.x = dx;
	obj.y = dy;
}

// Initializes the maze, places outer walls around the edges.
void setMaze(char maze[][MAZE_WIDTH])
{
	for (int row = 0; row < MAZE_HEIGHT; row++)
	{
		for (int col = 0; col < MAZE_WIDTH; col++)
		{
			maze[row][col] = EMPTY;

			// Place the top and bottom walls.
			maze[0][col] = WALL;
			maze[MAZE_HEIGHT - 1][col] = WALL;
		}

		// Place the left and right walls.
		maze[row][0] = WALL;
		maze[row][MAZE_WIDTH - 1] = WALL;
	}
}

// Copies the contents of the maze array into the grid array.
void setGrid(char grid[][MAZE_WIDTH], char maze[][MAZE_WIDTH])
{
	for (int row = 0; row < MAZE_HEIGHT; row++)
	{
		for (int col = 0; col < MAZE_WIDTH; col++)
		{
			grid[row][col] = maze[row][col];
		}
	}
}

// Create NUM_ZOMBIES amount of zombie objects and place them on the grid.
void initializeZombies(std::vector<Object> &zombies, char grid[][MAZE_WIDTH])
{
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 2; i++)
		{
			int x = 1 + i * (MAZE_WIDTH - 3);
			int y = 1 + j * (MAZE_HEIGHT - 3);
			zombies.push_back({ ZOMBIE, x, y, x, y });
			placeObject(zombies.at(j * 2 + i), grid);
		}
	}
}

// Create a given amount of pill objects and place them on the grid.
void initializePills(std::vector<Object> &pills, const int amount, char grid[][MAZE_WIDTH])
{
	for (int i = 0; i < amount; i++)
	{
		pills.push_back({ PILL, 0, 0 });
		spawnObject(pills.at(i), grid);
	}
}

void initializeHoles(char maze[][MAZE_WIDTH], const int amount)
{
	for (int i = 0; i < amount; i++)
	{
		// This is a dummy object so I can reuse the spawnObject function,
		Object hole = { HOLE, 0, 0 };
		spawnObject(hole, maze);
	}
}

// Draws the grid by looking at each cell and setting
// an appropriate colour for each character.
void drawGrid(char grid[][MAZE_WIDTH])
{
	// Offset the grid slightly.
	GotoXY(0, 2);

	SelectBackColour(clBlack);
	for (int row = 0; row < MAZE_HEIGHT; row++)
	{
		for (int col = 0; col < MAZE_WIDTH; col++)
		{
			switch (grid[row][col])
			{
				case PILL:		SelectForeColour(clDarkYellow); break;
				case ZOMBIE:	SelectForeColour(clGreen);	break;
				case HOLE:		SelectForeColour(clDarkRed); break;
				case PLAYER:	SelectForeColour(getPlayerColour()); break;
				default:		SelectForeColour(clWhite);
			}

			PutCh(grid[row][col]);
		}
		std::cout << "\n";
	}
}

// Return an x and y offset for the player based on the
// user input.
void setPlayerDirection(const int key, int &dx, int &dy)
{
	assert(isArrowKey(key));

	dx = 0, dy = 0;
	switch (key)
	{
		case KEY_UP:    dy = -1; break;
		case KEY_DOWN:  dy =  1; break;
		case KEY_LEFT:  dx = -1; break;
		case KEY_RIGHT: dx =  1; break;
	}
}

// Given the player's coordinates, work out which direction
// the zombie should move to chase the player.
void setZombieDirection(Object &zombie, const Object &player, int &dx, int &dy)
{
	dx = 0, dy = 0;
	// Will make the following algorithm easier to read.
	int px = player.x;
	int py = player.y;
	int zx = zombie.x;
	int zy = zombie.y;

	if (px > zx)
		dx = 1;
	else if (px < zx)
		dx = -1;
	if (py > zy)
		dy = 1;
	else if (py < zy)
		dy = -1;

	//Invert zomibes direction when magic powaaaa
	if (MAGICDURATION >= 1) {
		dy *= -1;
		dx *= -1;
	}

}

void movePlayer(
	const int key,
	Object &player,
	const char grid[][MAZE_WIDTH])
{
	int dx, dy;

	// Determine which way to move the player.
	setPlayerDirection(key, dx, dy);

	// Set the player's offsets.
	int playerDestX = player.x + dx;
	int playerDestY = player.y + dy;

	if (grid[playerDestY][playerDestX] != WALL)
	{
		moveObject(player, playerDestX, playerDestY);
	}
}

void moveZombies(
	std::vector<Object> &zombies,
	const Object &player,
	const char grid[][MAZE_WIDTH])
{
	int dx, dy;

	// Determine which direction to move each zombie.
	for (int i = 0; i < zombies.size(); i++)
	{
		Object &curZombie = zombies.at(i);

		setZombieDirection(curZombie, player, dx, dy);

		int zombieDestX = curZombie.x + dx;
		int zombieDestY = curZombie.y + dy;
	
		if (grid[zombieDestY][zombieDestX] != WALL)
		{
			moveObject(curZombie, zombieDestX, zombieDestY);
		}
	}
}

// Determines what to do with the player based on the cell
// it is currently located on.
void updatePlayer(
	Object &player, 
	int &playerLives, 
	std::string &playerMessage, 
	const char grid[][MAZE_WIDTH],
	std::vector<Object> 
	&pills, std::vector<Object> &zombies)
{
	switch (grid[player.y][player.x])
	{
		case WALL:
		{
			playerMessage = "CANNOT MOVE THERE!";
		} break;

		// Only activate a pill if there is no zombie on it,
		// but still remove it either way.
		case PILL:
		{
			if (findObject(zombies, player.x, player.y) == -1)
			{
				playerLives++;
				playerMessage = "ATE A PILL!";
				addMagicEffect();
			}

			// Remove the pill from its container.
			pills.erase(pills.begin() + findObject(pills, player.x, player.y));
		} break;

		// Moving onto a hole loses a life.
		case HOLE:
		{
			playerLives--;
			playerMessage = "FELL IN A HOLE!";
		} break;
	}
}

// Check a zombie against every other zombie and if they
// collide then respawn them both.
void checkZombieZombieCollision(
	std::vector<Object> &zombies,
	const int zombieIndex,
	Object &zombie,
	std::string &zombieMessage)
{
	// Check this zombie against every other zombie and determine
	// if they occupy the same cell, if so, respawn them both.
	for (int j = 0; j < zombies.size(); j++)
	{
		// We don't want to check a zombie against itself.
		if (j != zombieIndex)
		{
			if (zombies[j].x == zombie.x && zombies[j].y == zombie.y)
			{
				moveObject(zombie, zombie.sx, zombie.sy);
				moveObject(zombies[j], zombies[j].sx, zombies[j].sy);
				zombieMessage = "BUMPED INTO ANOTHER ZOMBIE!";
			}
		}
	}
}

// Determines what to do with a given zombie based on the cell
// it is currently located on.
void updateZombie(
	const int zombieIndex,
	Object &zombie,
	std::string &zombieMessage,
	std::vector<Object> &zombies,
	Object &player,
	int &playerLives,
	const char grid[][MAZE_WIDTH])
{
	switch (grid[zombie.y][zombie.x])
	{
		// If the zombies destination is a pill and the player's destination
		// is the same then respawn the zombie.
		case PILL:
		{
			if (zombie.x == player.x && zombie.y == player.y)
			{
				moveObject(zombie, zombie.sx, zombie.sy);
				zombieMessage = "ZOMBIE ON PILL & PLAYER!";
			}
		} break;

		// If the zombies destination is a hole, kill it.
		case HOLE:
		{
			zombies.erase(zombies.begin() + zombieIndex);
			zombieMessage = "FELL IN A HOLE";
		} break;

		default:
		{
			// As the player and other zombies may have moved, we can't 
			// use the grid to check their location.
			if (zombie.x == player.x && zombie.y == player.y)
			{
				//Check to see if player is super sayan
				if (MAGICDURATION >= 1)
				{
					zombies.erase(zombies.begin() + zombieIndex);
					zombieMessage = "ZOMBIE DIED BECAUSE PLAYER IS A FIGHTY BOI";
				}
				else {
					playerLives--;
					moveObject(zombie, zombie.sx, zombie.sy);
					zombieMessage = "HURT PLAYER!";
				}

			}

			checkZombieZombieCollision(zombies, zombieIndex, zombie, zombieMessage);
		}
	}
}

// Determines where to move the player and each zombie.
// Once their destinations have been calculated, handle
// any collisions with other objects and/or tiles.
void updateGameData(
	const int key, 
	char grid[][MAZE_WIDTH],
	Object &player, 
	int &playerLives,
	std::string &playerMessage,
	std::string &zombieMessage,
	std::vector<Object> &pills,
	std::vector<Object> &zombies,
	const bool isZombieFreeze)
{
	movePlayer(key, player, grid);
	updatePlayer(player, playerLives, playerMessage, grid, pills, zombies);

	if (!isZombieFreeze)
	{
		moveZombies(zombies, player, grid);
	}

	for (int i = 0; i < zombies.size(); i++)
	{
		updateZombie(i, zombies.at(i), zombieMessage, zombies, player, playerLives, grid);
	}

	updateMagicEffect();
}

// Copies the maze into the grid and places
// all game objects.
void updateGrid(
	char grid[][MAZE_WIDTH],
	char maze[][MAZE_WIDTH],
	Object &player,
	const std::vector<Object> &pills,
	const std::vector<Object> &zombies)
{
	setGrid(grid, maze);

	for (int i = 0; i < pills.size(); i++)
	{
		placeObject(pills.at(i), grid);
	}

	placeObject(player, grid);

	for (int i = 0; i < zombies.size(); i++)
	{
		placeObject(zombies.at(i), grid);
	}
}

int main()
{
	// This seed will be used to reload the game
	// from a previous session.
	const int seed = time(NULL);
	srand(seed);

	char maze[MAZE_HEIGHT][MAZE_WIDTH];
	char grid[MAZE_HEIGHT][MAZE_WIDTH];
	setMaze(maze);

	const int NUM_HOLES = 8;
	initializeHoles(maze, NUM_HOLES);

	// The grid must be set now so it can be used
	// when initializing the game objects.
	setGrid(grid, maze);

	Object player = { PLAYER, 0, 0 };
	int playerLives = 3;
	spawnObject(player, grid);

	const int NUM_PILLS = 12;
	std::vector<Object> pills;
	initializePills(pills, NUM_PILLS, grid);

	std::vector<Object> zombies;
	initializeZombies(zombies, grid);

	std::string curDate, curTime;
	curDate = GetDate();
	curTime = GetTime();

	std::string playerMessage, zombieMessage, miscMessage;

	displayMainMenu(curDate, curTime);

	std::string playerName;
	getline(std::cin, playerName);

	const int playerHighscore = loadScore(playerName);
	bool hasCheated = false;	
	bool freezeZombies = false;

	bool gameActive = true;
	bool hasWon = false;


	// Do this now so the player can see the game before any input.
	ClearScreen();
	drawGrid(grid);
	drawUI(curDate, curTime, playerName, playerLives, playerHighscore, pills.size());
	drawMessages(playerMessage, zombieMessage, miscMessage);

	//	Initialise a new thread for the timer.
	thread timer(updateTimer);

	int key = 0;
	while (key != 'Q' && gameActive)
	{	
		playerMessage = zombieMessage = miscMessage = "";

		curDate = GetDate();
		curTime = GetTime();

	
		key = getKeyPress();
		if (isArrowKey(key))
		{
			updateGameData(key, grid, player, playerLives, playerMessage, zombieMessage, pills, zombies, freezeZombies);
			updateGrid(grid, maze, player, pills, zombies);
		}
		else if (key != 'Q')
		{
			if (isCheatKey(key))
			{
				hasCheated = true;
				miscMessage = "CHEAT ACTIVATED: ";
				switch (key)
				{
					case CHEAT_FREEZE:  miscMessage += "FREEZE"; freezeZombies = !freezeZombies;	break;
					case CHEAT_FEED:	miscMessage += "FEED"; pills.clear();						break;
					case CHEAT_KILL:	miscMessage += "KILL"; zombies.clear();						break;
				}
			}
			else
			{
				playerMessage = "INVALID COMMAND!";
			}
		}

		if (hasCheated)
		{
			displayMessage("CHEATS ACTIVATED", 0, 6 + MAZE_HEIGHT, clBlack, clDarkRed);
		}

		if (playerLives <= 0)
		{
			gameActive = false;
		}
		else if (pills.size() == 0 && zombies.size() == 0)
		{
			hasWon = true;
			gameActive = false;
		}

		ClearScreen();

		drawGrid(grid);
		drawUI(curDate, curTime, playerName, playerLives, playerHighscore, pills.size());
		drawMessages(playerMessage, zombieMessage, miscMessage);

	}

	//Detatch timer thread


	// Display final messages based on whether the player won, cheated, etc...
	displayMessage("THANK YOU FOR PLAYING! GAME LASTED " + std::to_string(TIMER) + " seconds", 50, 25, clBlack, clWhite);
	FINISHED = true;
	// If the player has won then display how many lives they have
	// remaining.
	if (hasWon)
	{
		displayMessage("YOU HAVE WON!", 50, 19, clBlack, clDarkGreen);
		displayMessage("WITH " + std::to_string(playerLives) + " REMAINING LIVES!",
						50, 20, clBlack, clDarkGreen);
	}
	else
	{
		displayMessage("YOU HAVE LOST!", 50, 19, clBlack, clDarkRed);
	}

	// If the player didn't cheat, won and beat their highscore
	// then save it. A new player's highscore was set to -1 before the game started
	// so their score will be saved regardless (If they didn't cheat and won).
	if (hasWon)
	{
		std::string str;
		if (playerLives > playerHighscore && !hasCheated)
		{
			str = "NEW HIGHSCORE!SCORE UPDATED...";
			saveScore(playerName, playerLives);
		}
		else
		{
			str = "SCORES WERE NOT UPDATED (CHEATS ACTIVE)";
		}

		displayMessage(str, 50, 26, clBlack, clDarkRed);
	}


	// Wait for the user to enter any key to exit.
	std::cin.get();

	return (0);
}