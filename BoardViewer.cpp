/*

Title: Helper2 (CPSC 427)
Date: 07-December-2012
File: BoardViewer.cpp
Author: Rosie Buchanan
Version: 2.0

Helper: Sudoku helper application that can be run by a user from the command line that will let him or her interactively solve a Sudoku puzzle. The program stores information for a sudoku board by storing information for the individual sudoku squares. Information stored for each square includes row and column position within the board, the possibility set of marks, the current mark, and whether or not the mark is immutable. Additional information is stored for the board itself: the number of rows, the number of columns, and the total number of squares. Clusters, a set of squares that must be filled in with distinct marks, has been introduced. This program handles three types of classical Sudoku clusters: rows, columns and squares. Each time a mark is recorded for a square, the mark is removed from the possibility sets of the the other squares in each of the clusters it is contained in. The board now prints the Board nicely using Unicode characters (U+2500 code chart). Users are able to use undo and redo functions, and add or erase individual marks.

BoardViewer: Derived from provided class Viewer that provides implementations for the pure virtual functions in the base class. It allows for the board to be printed in Unicode. It is given access to a Board object.

*/

#include "BoardViewer.hpp"

//constructs Board object by extending Viewer class
BoardViewer::BoardViewer(Board* board) : Viewer(board->Board::getRows(), board->Board::getCols())
{
	brd = board; //store board pointer
}

//returns the char to display in the center of square in the specified row and column of the Sudoku board
char BoardViewer::getMarkChar(int row, int col) const
{
	int mk = brd->Board::boardGetMark(row, col); //get mark for row and col
	char m = ' '; // display no mark if mark is blank
	if (mk != -1)
	{
		m = (char) (mk + 48); //convert int mark to char
	}
	return m;
}

//returns the string to display on the top row of the specified square found by using row and col
string BoardViewer::getPossibleSetString(int row, int col) const
{	
	string s = "         "; //initialize possibleSet as blank
	//if (brd->Board::boardGetMark(row, col) == -1) // show possibleSet only if blank mark is stored
	//{	
		s = brd->Board::boardGetPossString(row, col); //get possibleSet
//	}
	return s;
}
