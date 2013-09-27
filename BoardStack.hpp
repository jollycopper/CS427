/*
Title: Helper2 (CPSC 427)
Date: 07-December-2012
File: BoardStack.hpp
Author: Rosie Buchanan
Version 2.0

Helper: Sudoku helper application that can be run by a user from the command line that will let him or her interactively solve a Sudoku puzzle. The program stores information for a sudoku board by storing information for the individual sudoku squares. Information stored for each square includes row and column position within the board, the possibility set of marks, the current mark, and whether or not the mark is immutable. Additional information is stored for the board itself: the number of rows, the number of columns, and the total number of squares. Clusters, a set of squares that must be filled in with distinct marks, has been introduced. This program handles three types of classical Sudoku clusters: rows, columns and squares. Each time a mark is recorded for a square, the mark is removed from the possibility sets of the the other squares in each of the clusters it is contained in. The board now prints the Board nicely using Unicode characters (U+2500 code chart). Users are able to use undo and redo functions, and add or erase individual marks.

BoardStack: A stack of BoardState pointers used to implement the undo and redo stacks. It is derived vector<BoardState⋆> and should support the usual stack operations as well as a flush operation that empties out the stack. It is derived from vector, but hides unneccessary public functions.

*/

#include "BoardState.hpp"

class BoardStack : vector<BoardState*> {
//extends vector of pointers to BoardStates
typedef vector<BoardState*> stack;

public:
	//push state s onto stack
	void push(BoardState* s);
	
	//pop state from stack and return popped state
	BoardState* pop();

	//peek at state at top of stack
	BoardState* peek();

	//empty stack
	void flush();
	
	//check if stack is empty
	bool empty();

	//serialize given stack into output stream
	ostream& serialize(ostream& out);
	
	//de-serialize stack from stream
	ifstream& realize(int r, int c, ifstream& file);
};
