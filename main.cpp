/*

Title: Helper2 (CPSC 427)
Date: 07-December-2012
File: main.cpp
Author: Rosie Buchanan
Version: 5.0

Helper: Sudoku helper application that can be run by a user from the command line that will let him or her interactively solve a Sudoku puzzle. The program stores information for a sudoku board by storing information for the individual sudoku squares. Information stored for each square includes row and column position within the board, the possibility set of marks, the current mark, and whether or not the mark is immutable. Additional information is stored for the board itself: the number of rows, the number of columns, and the total number of squares. Clusters, a set of squares that must be filled in with distinct marks, has been introduced. This program handles three types of classical Sudoku clusters: rows, columns and squares. Each time a mark is recorded for a square, the mark is removed from the possibility sets of the the other squares in each of the clusters it is contained in. The board now prints the Board nicely using Unicode characters (U+2500 code chart). Users are able to use undo and redo functions, and add or erase individual marks.

main: This runs the Helper program. Two arguments must be given in the command line: the number of rows and the number of columns, in that order. It starts a new game, which includes the commands and prompts within the Game class. When the Board is shown, it is printed nicely in Unicode form, with the mark and possibility set shown for each square.

*/

#include "BoardViewer.hpp"
#include "Game.hpp"



// contains Board test program
void run(int argc, char *argv[])
{
	// NOTE: assuming number of rows and columns is each less than 9, since the possibility set is only 1-9.
	if (argc != 3) { throw Fatal("usage: rows cols"); }	
	char rowchar = *argv[1]; // store number of rows
	int rows = (rowchar - '0'); // convert to int
	char colchar = *argv[2]; // store number of columns
	int cols = (colchar - '0'); // convert to int
	//run game of user inputted size
	Game::run(rows,cols);
}

int main(int argc, char *argv[]) {  
    banner();   // print header banner
    try {  
        run(argc, argv);  // run Board test program
    }  
	// error checking
    catch (Fatal& e) {  
        cerr << "Catching Fatal exception\n" << e.what() << endl;  
    }  
    catch (...) {  
        cerr << "Uncaught exception" << endl;  
    }  
    bye();  //terminate and print termination message
}

//See file "sampleoutput.txt" for sample output.
