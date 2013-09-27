/*
Title: Helper2 (CPSC 427)
Date: 07-December-2012
File: Game.hpp
Author: Rosie Buchanan
Version: 2.0

Helper: Sudoku helper application that can be run by a user from the command line that will let him or her interactively solve a Sudoku puzzle. The program stores information for a sudoku board by storing information for the individual sudoku squares. Information stored for each square includes row and column position within the board, the possibility set of marks, the current mark, and whether or not the mark is immutable. Additional information is stored for the board itself: the number of rows, the number of columns, and the total number of squares. Clusters, a set of squares that must be filled in with distinct marks, has been introduced. This program handles three types of classical Sudoku clusters: rows, columns and squares. Each time a mark is recorded for a square, the mark is removed from the possibility sets of the the other squares in each of the clusters it is contained in. The board now prints the Board nicely using Unicode characters (U+2500 code chart). Users are able to use undo and redo functions, and add or erase individual marks.

Game: The top-level class that runs the game. It owns the board, the undo and redo stacks, and the board viewer. It implements the main command loop and the code for the individual commands, and each command is implemented by a separate member function, as well as the command dispatcher (the code that reads the command letter and calls the appropriate command function).

*/

#pragma once
#include "tools.hpp"
#include "BoardViewer.hpp"
#include "BoardStack.hpp"

class Game {
private:
	//dimensions of board
	int rows;
	int cols;

	//stacks to store undid/redid moves in form of BoardStates
	BoardStack undo;
	BoardStack redo;

	//print welcome msg
	void welcome();

	//print menu of possible commands
	void menu();

	//command prompt
	void askCommand();

	//create and initialize board with no marks, full possibility sets, and mutable squares 
	Board* createBoard();

	//utilize BoardViewer to display board in nice format
	void showBoard(Board* b);

	//load board from file
	Game* load(Board* b);

	//check range of rows/cols/marks
	 bool checkRange(int r, int c, int m);

	//change row/col/mark char into integer
	int readRCM(char c);

	//get row, column, and value of mark to be removed and remove mark from board, updating square info
	void remove(Board* b);

	//get row, column, and value of mark to be added and add mark from board, updating square info
	void mark(Board* b);

	//undo and return board to state of last move
	void undoF(Board* b);

	//redo and return board to state before undo command was called
	void redoF(Board* b);

	//quit and destroy game
	void quit(Board* b);

	//empty redo stack
	void flushRedo();
	
	//save game to file in serialized form
	void save();
	
	//serialize game to output stream
	ostream& serialize(ostream& out);

	//de-serialize game from input file stream and load as new game
	Game* realize(Board* b, ifstream& file, string filename);

public:
	// create game of dimensions (r, c)
	Game(int r, int c);

	//control center for game: prompt for and follow commands
	Game* runGame();
	
	//run game
	static void run(int nr, int nc);
}; 

