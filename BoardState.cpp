/*
Title: Helper2 (CPSC 427)
Date: 07-December-2012
File: BoardState.cpp
Author: Rosie Buchanan
Version: 2.0

Helper: Sudoku helper application that can be run by a user from the command line that will let him or her interactively solve a Sudoku puzzle. The program stores information for a sudoku board by storing information for the individual sudoku squares. Information stored for each square includes row and column position within the board, the possibility set of marks, the current mark, and whether or not the mark is immutable. Additional information is stored for the board itself: the number of rows, the number of columns, and the total number of squares. Clusters, a set of squares that must be filled in with distinct marks, has been introduced. This program handles three types of classical Sudoku clusters: rows, columns and squares. Each time a mark is recorded for a square, the mark is removed from the possibility sets of the the other squares in each of the clusters it is contained in. The board now prints the Board nicely using Unicode characters (U+2500 code chart). Users are able to use undo and redo functions, and add or erase individual marks.

BoardState: A copiable class used to store the current state of the board. This consists of copies of the Square objects in an array that comprise the board but not the clusters and other information that is in BSquare (and which doesn’t change once the board has been constructed).

*/

#include "BoardState.hpp"

//construct board state of dimensions (r, c) with all marks empty but possible
BoardState::BoardState(int r, int c): rows(r), cols(c)
{
	//allocate space for array of squares
	arraySq = (new Square[r*c]);

	//loop through squares and initialize each to empty
	for (int i = 0; i < r*c; i++)
	{
		arraySq[i].mark = -1;
		arraySq[i].possibleSet = 0x000003fe;
		arraySq[i].immutable = false;
	}
}

//construct board state from Board b by extracting info for each square
BoardState::BoardState(Board* b): rows(b->Board::getRows()), cols(b->Board::getCols())
{
	//allocate space for array of squares
	arraySq = (new Square[b->Board::getTotal()]);
	//loop through squares and copy needed info for each from corresponding board BSquare
	for (int i = 0; i < b->Board::getTotal(); i++)
	{
		arraySq[i] = b->Board::getSquare(i);
		//arraySq[i].mark = b->Board::boardGetMark(i);
		//arraySq[i].possibleSet = b->Board::boardGetPossible(i);
		//arraySq[i].immutable = b->Board::boardGetImmutable(i);
	}
}

//update board b from boardstate
void BoardState::fromBoardState(Board* b)
{
	//loop through squares and copy updated info to Board b
	for (int i = 0; i <rows*cols; i++)
	{
		Square s = arraySq[i];
		b->Board::copySquare(i, s);
	}
}

//delete board state
void BoardState::delBoardState()
{
	delete[] arraySq; //delete array of squares
}

//serialize given stack into output stream
ostream& BoardState::serialize(ostream& out)
{
	//serialize each square of board state
	for (int i = 0; i <rows*cols; i++)
	{
		arraySq[i].Square::serialize(out);
		out << endl;
	}
return out;
}

//de-serialize stack from stream
ifstream& BoardState::realize(ifstream& file)
{
	//de-serialize each square of board state
	for (int i = 0; i<rows*cols; i++)
	{
		arraySq[i].Square::realize(file);
	}
	return file;
}
