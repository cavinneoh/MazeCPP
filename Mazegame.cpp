#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <regex>
#include <cstdlib>
#include <ctime>
#include "Mazegame.h"

void printErrorMessage(int errorCode) {
	//prints error message after error is caught
	cout << "ERROR! ";
	switch (errorCode) {
	case 10: cout << "Unable to locate file." << endl;
		break;
	case 20: cout << "Text file data is invalid or wrongly formatted." << endl;
		break;
	}
}

void printFancyMessage(string message, char symbol) {
	//prints message with symbol parameter characters above and below it
	for (int i = 0; i < message.length(); i++) {
		cout << symbol;
	}
	cout << endl;
	cout << message << endl;
	for (int i = 0; i < message.length(); i++) {
		cout << symbol;
	}
	cout << endl;
}

void quitProgram() {
	//gracefully exit program when called
	printFancyMessage("You have chosen to quit The Maze Game.", '#');
	system("pause");
	exit(0);
}

char setMazeLayout() {

	cout << "Welcome to The Maze Game. Enter (A) to play in a fixed layout" << endl;
	cout << "OR enter (R) to play in a randomly generated maze " << endl;
	cout << "OR enter (Q) to quit =( " << endl;
	string inputString = "";
	char choice;

	do {//capture input as string, then check first character of input
		cout << endl << "Please enter (A), (R) or (Q): ";
		getline(cin, inputString);
		choice = toupper(inputString[0]);

		if (choice == 'Q') {
			quitProgram();
		}
		//loop if input is 2 or more characters
	} while (inputString.length() > 1 || (choice != 'A' && choice != 'R'));

	return choice;
}

bool isValidTextFileInput(string textLine) {

	/*regular expression to check for data format
	whitespaces are removed before passing to this function*/
	return (regex_match(textLine, regex("^[NESWT]=[A-Z]$")));
}
string getFileName(char letter) {
	// returns location of file using roomLabel as parameter

	string textFile = "";
	textFile += "gamedata/room";
	textFile += letter;
	textFile += ".txt";
	return textFile;
}
Exit::Exit(char dir, char next) {
	//constructor of Exit class
	exitDirection = new char;
	*exitDirection = dir;
	nextRoom = new char;
	*nextRoom = next;
}
void Exit::printDirection() {
	//function to print direction that will be called 
	//once for every exit object within room object
	switch (*exitDirection) {
	case 'N':
		cout << "(N)orth";
		break;
	case 'E':
		cout << "(E)ast";
		break;
	case 'S':
		cout << "(S)outh";
		break;
	case 'W':
		cout << "(W)est";
		break;
	case 'T':
		cout << "through the (T)rapdoor";
		break;
	}
}

char Exit::getExitDirection() {
	//getter function
	return *exitDirection;
}

char Exit::getNextRoom() {
	//getter function
	return *nextRoom;
}


Room::Room(char label) {

	roomLabel = new char;
	*roomLabel = toupper(label);
	string roomFile = getFileName(*roomLabel);


	ifstream myFile(roomFile);	//target file to open
	string line;	//line variable to traverse content of file
	try {
		if (!myFile.is_open()) { //throws error code if unable to locate file
			throw 10;
		}
		//loop to traverse file contents
		while (getline(myFile, line))
		{
			transform(line.begin(), line.end(), line.begin(), ::toupper);
			// change input of data file to uppercase

			line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
			//removing white spaces identified with isspace from every line

			if (line != "") {
				if (!isValidTextFileInput(line)) {
					//check if file dat is in a valid format
					throw 20;
				}
				else {
					char dir = line[line.find('=') - 1];
					//capture direction of next room based on data format
					char next = line[line.find('=') + 1];
					//capture label of next room based on data format
					exits[numOfExits] = new Exit(dir, next);
					// create exit object based on direction and label captured
					numOfExits++;
				}
			}
		}
		if (numOfExits == 0) { throw 20; }
		myFile.close();
	}
	catch (int errorCode) {
		printErrorMessage(errorCode);
		cout << "Unable to execute program" << endl;
		system("pause");
		exit(0);
	}
}

char Room::getRoomLabel() {
	//getter method
	return *roomLabel;
}

char Room::renderRoom() {

	cout << "You are in room " << getRoomLabel();
	cout << " of a maze of twisty little passages, all alike." << endl;
	cout << "You can go ";

	for (int i = 0; i < numOfExits; i++) {
		if (i > 0) {
			cout << ", ";
		}
		exits[i]->printDirection();
	}//print directions of exits available in the room.
	cout << ", or (Q)uit" << endl;

	string stringInput;
	getline(cin, stringInput);
	char input = toupper(stringInput[0]);
	//capture input as string, then capture first character
	cout << endl;

	if (stringInput.length() == 1) {
		//accept 1 character input only
		if (input == 'Q') {
			quitProgram();
		}//no else statement because it is redundant
		for (int i = 0; i < numOfExits; i++) {
			if (exits[i]->getExitDirection() == input) {
				//if input matches available exit directions
				cout << "Going ";
				exits[i]->printDirection();
				cout << "...\n\n";
				char nextRoomLabel = exits[i]->getNextRoom();
				return nextRoomLabel;
			}
		}
	}
	cout << "Invalid input. Please try again." << endl << endl;
	return renderRoom();
	//recursively repeat the function it receives a valid input
}

EscapeRoom::EscapeRoom(char label) : Room(label) {
}

char EscapeRoom::renderRoom() {

	cout << "You are in room " << getRoomLabel() << "." << endl;
	cout << "You see a light at the end of the tunnel." << endl << endl;
	printFancyMessage("Congratulations! You have escaped the maze.", '*');

	return getRoomLabel();
}

Maze::Maze(char layout) {

	firstRoomLabel = new char;
	*firstRoomLabel = toupper(layout);

	if (*firstRoomLabel == 'A') {//create maze layout, starting from room A
		escapeRoomLabel = new char;
		*escapeRoomLabel = 'N';

		cout << "You have been placed in a maze of layout A" << endl << endl;
	}
	else if (*firstRoomLabel == 'R') {//create maze layout, starting from room R
		escapeRoomLabel = new char;
		*escapeRoomLabel = 'O';

		cout << "You have been placed in a randomly generated maze. " << endl << endl;
		srand(time(0));	//seed to generate different random number for each run

		for (char letter = *escapeRoomLabel; letter <= 'Z'; letter++) {
			//generate random room exits from roomO to roomZ
			generateRandomRoomFile(letter);
		}
	}
	currentRoom = new Room(*firstRoomLabel);
}

char Maze::getEscapeRoomLabel() {
	//getter method
	return *escapeRoomLabel;
}

char Maze::getCurrentRoomLabel() {
	//uses getter method of Room Object
	return currentRoom->getRoomLabel();
}

void Maze::leaveCurrentRoom() {
	currentRoom = new Room(currentRoom->renderRoom());
	numOfMoves++;

	if (currentRoom->getRoomLabel() == *escapeRoomLabel) {
		currentRoom = new EscapeRoom(getCurrentRoomLabel());
		currentRoom->renderRoom();
		cout << "Moves taken to escape maze: " << numOfMoves << endl;
	}
}

void Maze::generateRandomRoomFile(char label) {

	ofstream writeFile;
	string fileName = getFileName(label);
	writeFile.open(fileName);
	//create blank text file (overwrites old file)
	char exitDirections[] = { 'N', 'E', 'S', 'W' };
	char vitalRooms[] = { 'P', 'Q', 'R' };
	//vitalRooms form the base of the maze
	char randomRooms[] = { 'S', 'T' , 'U', 'V', 'W', 'X', 'Y', 'Z' };
	//randomRooms have a exit that connects to a vitalRooms

	int randomNumOfExits = 2 + rand() % 3;
	//a random number between 2 and 4
	int vitalExitPos = rand() % randomNumOfExits;
	//randomize a position to place vital exit
	//in the range of the randomNumOfExits
	int randomIndex;
	string tempDir;
	string tempLabel;
	string vitalExit;
	string exitDirAndLabel;

	randomIndex = rand() % sizeof(exitDirections);
	//a random number in the range of exitDirections indexes
	tempDir = exitDirections[randomIndex];
	exitDirections[randomIndex] = ' ';
	//replace selected direction with space character
	//to avoid multiple exits of the same direction

	if (label == 'P' || label == 'Q' || label == 'R') {
		//3 vitalRooms that form a connection to exit room O

		switch (label) {
		case 'R':
			tempLabel = 'Q';
			break; //connect R to Q
		case 'Q':
			tempLabel = 'P';
			break; //connect Q to P
		case 'P':
			tempLabel = 'O';
			break; //connect P to exit room O
		}
	}
	else {
		randomIndex = rand() % sizeof(vitalRooms);
		tempLabel = vitalRooms[randomIndex];
		//randomly connect to a vital room
	}
	vitalExit = tempDir + " = " + tempLabel;

	//for loop to write generated exits to file
	for (int i = 0; i < randomNumOfExits; i++) {
		if (i == vitalExitPos) {
			//create in a random order of vital exit
			writeFile << vitalExit << endl;
		}
		else {
			do {	//loop to select another direction if empty space is selected
				randomIndex = rand() % sizeof(exitDirections);

				if (exitDirections[randomIndex] != ' ') {
					tempDir = exitDirections[randomIndex];
					exitDirections[randomIndex] = ' ';
					//replace selected direction with empty space
					//to avoid multiple exits of the same direction
					break;
				}
			} while (exitDirections[randomIndex] == ' ');

			do {	//loop to select another room if empty space is selected
				randomIndex = rand() % sizeof(randomRooms);

				if (randomRooms[randomIndex] != ' ' && randomRooms[randomIndex] != label) {
					//does not allow an exit to current room

					tempLabel = randomRooms[randomIndex];
					randomRooms[randomIndex] = ' ';
					//replace selected direction with empty space
					//to avoid multiple exits to a similar room
					break;
				}
			} while (randomRooms[randomIndex] == ' ');

			exitDirAndLabel = tempDir + " = " + tempLabel;
			writeFile << exitDirAndLabel << endl;
		}
	}//end of for loop
	writeFile.close();
}//end of generateRandomRoomFile()