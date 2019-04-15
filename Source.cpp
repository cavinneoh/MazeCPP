#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <regex>
#include <cstdlib>
#include <ctime>
#include "Mazegame.h"

int main() {
	
	Maze* mazeGame = new Maze(setMazeLayout());
	//create Maze object

	while (mazeGame->getCurrentRoomLabel() != mazeGame->getEscapeRoomLabel()) {
		//loop until player reaches exit room
		mazeGame->leaveCurrentRoom();
	}
	system("pause");
	return 0;
}