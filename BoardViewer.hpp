/*

Title: Helper2 (CPSC 427)
Date: 07-December-2012
File: BoardViewer.hpp
Author: Rosie Buchanan
Version: 2.0

Helper: Sudoku helper application that can be run by a user from the command line that will let him or her interactively solve a Sudoku puzzle. The program stores information for a sudoku board by storing information for the individual sudoku squares. Information stored for each square includes row and column position within the board, the possibility set of marks, the current mark, and whether or not the mark is immutable. Additional information is stored for the board itself: the number of rows, the number of columns, and the total number of squares. Clusters, a set of squares that must be filled in with distinct marks, has been introduced. This program handles three types of classical Sudoku clusters: rows, columns and squares. Each time a mark is recorded for a square, the mark is removed from the possibility sets of the the other squares in each of the clusters it is contained in. The board now prints the Board nicely using Unicode characters (U+2500 code chart). Users are able to use undo and redo functions, and add or erase individual marks.

BoardViewer: Derived from provided class Viewer that provides implementations for the pure virtual functions in the base class. It allows for the board to be printed in Unicode. It is given access to a Board object.

*/

#pragma once

#include "Viewer.hpp"
#include "Board.hpp"

class BoardViewer : public Viewer {
private:
	Board* brd; //given access to Board object
public:
	//constructs Board object by extending Viewer class
	BoardViewer(Board* board);
	//returns the char to display in the center of square in the specified row and column of the Sudoku board
	virtual char getMarkChar(int row, int col) const;
	//returns the string to display on the top row of the specified square found by using row and col
	virtual string getPossibleSetString(int row, int col) const;
};

