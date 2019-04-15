#ifndef MAZEGAME_H
#define MAZEGAME_H

#include <string>
using namespace std;

class Exit {

	char* exitDirection;
	char* nextRoom;

public:
	Exit(char dir, char next);

	char getExitDirection();
	char getNextRoom();
	void printDirection();
};

class Room {

	char* roomLabel;
	int numOfExits = 0;
	Exit* exits[5];

public:
	Room(char label);
	char getRoomLabel();
	virtual char renderRoom();
};

class EscapeRoom : public Room {
public:
	EscapeRoom(char label);
	char renderRoom();
};

class Maze {

	char* firstRoomLabel;
	char* escapeRoomLabel;
	Room* currentRoom;
	int numOfMoves = 0;

public:
	Maze(char layout);
	char getEscapeRoomLabel();
	char getCurrentRoomLabel();
	void leaveCurrentRoom();
	void generateRandomRoomFile(char label);
};

void printErrorMessage(int errorCode);
void printFancyMessage(string message, char symbol);
void quitProgram();
char setMazeLayout();
bool isValidTextFileInput(string textLine);
string getFileName(char letter);

#endif