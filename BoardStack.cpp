/*
Title: Helper2 (CPSC 427)
Date: 07-December-2012
File: BoardStack.cpp
Author: Rosie Buchanan
Version: 2.0

Helper: Sudoku helper application that can be run by a user from the command line that will let him or her interactively solve a Sudoku puzzle. The program stores information for a sudoku board by storing information for the individual sudoku squares. Information stored for each square includes row and column position within the board, the possibility set of marks, the current mark, and whether or not the mark is immutable. Additional information is stored for the board itself: the number of rows, the number of columns, and the total number of squares. Clusters, a set of squares that must be filled in with distinct marks, has been introduced. This program handles three types of classical Sudoku clusters: rows, columns and squares. Each time a mark is recorded for a square, the mark is removed from the possibility sets of the the other squares in each of the clusters it is contained in. The board now prints the Board nicely using Unicode characters (U+2500 code chart). Users are able to use undo and redo functions, and add or erase individual marks.

BoardStack: A stack of BoardState pointers used to implement the undo and redo stacks. It is derived vector<BoardState⋆> and should support the usual stack operations as well as a flush operation that empties out the stack. It is derived from vector, but hides unneccessary public functions.


*/
#include "BoardStack.hpp"

//push state s onto stack
void BoardStack::push(BoardState* s)
{
	stack::push_back(s);
}

//pop state from stack and return popped state
BoardState* BoardStack::pop()
{
	BoardState* popped;
	//check if stack is empty
	if (stack::empty())
	{
		popped = NULL;
	}
	//store pointer to top element, remove pointer from stack
	else
	{
		popped = back();
		pop_back();
	}
	return popped;
}

//peek at state at top of stack
BoardState* BoardStack::peek()
{
	BoardState* peek;
	peek = back();
	return peek;
}

//empty stack
void BoardStack::flush()
{
	while (!empty())
	{
		//store pointer to top element
		BoardState* popped;
		popped = back();
		//delete allocated items in boardstate
		if (popped != NULL)
		{
		popped->BoardState::delBoardState();
		}
		//pop
		pop_back();
		//delete whole element if stack not empty
		if (!empty())
		{
			delete popped;
		}
	}
}
//check if stack is empty
bool BoardStack::empty()
{
	bool isEmpty = false;
	if (stack::empty())
	{
		isEmpty = true;
	}
	return isEmpty;
}

//serialize stack to output stream
ostream& BoardStack::serialize(ostream& out)
{
	//output size of stack
	out << size() << endl;
	//serialize each board state on stack
	for (unsigned int i = 0; i < size(); i++)
	{
		at(i)->BoardState::serialize(out);
	}
return out;
}
//de-serialize stack from input stream file
ifstream& BoardStack::realize(int r, int c, ifstream& file)
{
	//store size of stack
	int size = 0;
	file >> size;
	for (int i = 0; i < size; i++)
	{
		//de-serialize each boardstate and push to stack
		BoardState* bs = new BoardState(r, c);
		try
		{
			bs->BoardState::realize(file);
			push(bs);
		}
		//delete stack if de-serialization fails
		catch(ifstream::failure e)
		{
			delete bs;
			throw;
		}
	}
	return file;
}

