/*

Title: Helper2 (CPSC 427)
Date: 07-December-2012
File: Board.hpp
Author: Rosie Buchanan
Version: 7.0

Helper2: Sudoku helper application that can be run by a user from the command line that will let him or her interactively solve a Sudoku puzzle. The program stores information for a sudoku board by storing information for the individual sudoku squares. Information stored for each square includes row and column position within the board, the possibility set of marks, the current mark, and whether or not the mark is immutable. Additional information is stored for the board itself: the number of rows, the number of columns, and the total number of squares. Clusters, a set of squares that must be filled in with distinct marks, has been introduced. This program handles three types of classical Sudoku clusters: rows, columns and squares. Each time a mark is recorded for a square, the mark is removed from the possibility sets of the the other squares in each of the clusters it is contained in. The board now prints the Board nicely using Unicode characters (U+2500 code chart). Users are able to use undo and redo functions, and add or erase individual marks. Added functionality allows users to save and load games, in a format that saves all past moves.

Board: Represents a Sudoku board with constant data members that contain the number of rows, number of columns, and the total number of squares. It has a constant data member pointer that points to a dynamically allocated array of BSquares. Board maintains a master list of all cluster objects.

*/

#pragma once
#include "tools.hpp"
#include "BSquare.hpp"


class Board
{
private:
	const int rows; // number of rows
	const int cols; // number of columns
	const int total; // total number of squares
	BSquare *brd; // points to dynamically allocated array of BSquares
	vector<Cluster*> clu; // list of all clusters on board
	
	//create clusters
	void makeRowClusters();
	void makeColClusters();
	void makeBoxClusters();

public:
	//determines linear subscript between 0 and (total-1) for Bsquare in position (r,c)
	//private function to map row/column coordinates to indices into a linear array
	int sub(int r, int c) const;

	// constructs a new sudoku board with nr rows, nc columns
	// performs dynamic allocation of board and initializes rows and column indices
	Board(int nr, int nc);
	
	// destructor uses delete to free storage allocated by Board(...)
	// now deletes Clusters too
	~Board();
	
	// writes current board marking to stream out in the same format as is used by the Board constructor when creating the Board
	ostream& writeMarks(ostream& out) const;
	
	// print function that prints the dimensions of the board of the first line
	// followed by one line for each square of the board
	//includes cluster info
	ostream& print(ostream& out) const;
	
	//reads board from file, stores marks
	bool readMarks(string boardFile); 
	
	//returns true if all unmarked squares have non-empty possibility lists
	bool isStillPossible() const;

	//return number of rows
	int getRows() const { return rows; }
	
	//return number of columns
	int getCols() const { return cols; }

	//return total number of squares
	int getTotal() const { return total; }

	// returns mark at given row and column of board
	int boardGetMark(int row, int col) const;

	//returns possibility set of given row and column of board
	string boardGetPossString(int row, int col) const;

	//unset possible mk from square at (row, col)
	void boardUnsetPoss(int row, int col, int mark);

	//initializes board to empty, with all marks possible
	void initializeBoard();

	//sets mark m at (r,c) and returns bool representing successfulness
	bool boardSetMark(int r, int c, int m);
	
	//copies square s to board at given index
	void copySquare(int index, Square s);

	//returns the square at a given index of the board
	Square getSquare(int i);

	//clears the possibility sets for marked squares of the board
	void clearPossSets();

};

//output operator extension
inline ostream& operator<<(ostream& out, const Board& sq) {
	return sq.print(out);
}
