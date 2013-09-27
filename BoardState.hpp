/*
Title: Helper2 (CPSC 427)
Date: 07-December-2012
Author: Rosie Buchanan
Version: 2.0

Helper: Sudoku helper application that can be run by a user from the command line that will let him or her interactively solve a Sudoku puzzle. The program stores information for a sudoku board by storing information for the individual sudoku squares. Information stored for each square includes row and column position within the board, the possibility set of marks, the current mark, and whether or not the mark is immutable. Additional information is stored for the board itself: the number of rows, the number of columns, and the total number of squares. Clusters, a set of squares that must be filled in with distinct marks, has been introduced. This program handles three types of classical Sudoku clusters: rows, columns and squares. Each time a mark is recorded for a square, the mark is removed from the possibility sets of the the other squares in each of the clusters it is contained in. The board now prints the Board nicely using Unicode characters (U+2500 code chart). Users are able to use undo and redo functions, and add or erase individual marks.

BoardState: A copiable class used to store the current state of the board. This consists of copies of the Square objects in an array that comprise the board but not the clusters and other information that is in BSquare (and which doesn’t change once the board has been constructed). A BoardState can be created from a board, or initialized to a clean BoardState.

*/

#include "Square.hpp"
#include "Board.hpp"

class BoardState {
private:
	//board dimensions
	int rows;
	int cols;
	//array of Square objects containing necessary info about state of board
	Square* arraySq;
public:
	//construct board state of dimensions (r, c) with all marks empty but possible	
	BoardState(int r, int c);
	
	//construct board state from Board b by extracting info for each square
	BoardState(Board* b);

	//update board b from boardstate
	void fromBoardState(Board* b);

	//delete board state
	void delBoardState();
	
	//serialize board state to output stream
	ostream& serialize(ostream& out);
	
	//de-serialize board state from input stream
	ifstream& realize(ifstream& file);
};

