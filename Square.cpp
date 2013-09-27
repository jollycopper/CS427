/*

Title: Helper2 (CPSC 427)
Date: 07-December-2012
File: Square.cpp
Author: Rosie Buchanan
Version: 5.0

Helper: Sudoku helper application that can be run by a user from the command line that will let him or her interactively solve a Sudoku puzzle. The program stores information for a sudoku board by storing information for the individual sudoku squares. Information stored for each square includes row and column position within the board, the possibility set of marks, the current mark, and whether or not the mark is immutable. Additional information is stored for the board itself: the number of rows, the number of columns, and the total number of squares. Clusters, a set of squares that must be filled in with distinct marks, has been introduced. This program handles three types of classical Sudoku clusters: rows, columns and squares. Each time a mark is recorded for a square, the mark is removed from the possibility sets of the the other squares in each of the clusters it is contained in. The board now prints the Board nicely using Unicode characters (U+2500 code chart). Users are able to use undo and redo functions, and add or erase individual marks.

Square: Sudoku helper program that stores information for a sudoku square. Implements single class Square which stores info about whether or not square is filled in, the number it is marked with, whether the mark was given by the puzzle, and the set of possibilities for the square.

Definitions of variables given in header file:
	int mark;				// current mark
	bool immutable;			// initial mark; can't be changed
	unsigned possibleSet;	// bit vector of possible marks
*/


#include "Square.hpp"

// constructs a blank or initially-marked square
// takes optional int argument
// int present: constructs square initally containing given argument, marked immutable
// int not present: constructs empty square marked mutable
Square::Square()
{
	possibleSet = 0x000003fe; // initialize possible set to all possibilities, 1-9
	immutable = false;
}

// constructs a square from an open input stream, compatible with serialize
Square::Square(istream& in)
{
	in >> mark; // store mark
	in >> immutable; // store immutable status
	in >> hex >> possibleSet >> dec; // store hex position
}

// fills in square with its argument
void Square::setMark(int mk)
{
	if (immutable || mk > maxMark || mk < minMark) // mark is already stored and cannot be changed or mark is outside acceptable range
	{
		throw Fatal("Tester catching Fatal exception");
	}
	if (mk == -1) // square is empty, store mark
	{
		mark = mk;
	}
	else
	{
		mark = mk;
	}
}

// returns true if mk is in the possibility set for the square
bool Square::isPossible(int mk) const
{
	bool isPoss = false;
	if (possibleSet&(1<<mk))
	{
		isPoss = true;
	}
	return isPoss;
}

// removes mk from the possibility set and returns true if the possibility set changed
bool Square::unsetPossible(int mk)
{
	bool changePossSet = false;
	if (possibleSet&(1<<mk)) // mk is in possibility set
	{
		possibleSet=possibleSet&~(1<<mk); // remove mk from possibility set
		changePossSet = true;
	}
	return changePossSet;
}

//inserts mk into the possibility set and returns true if the possibility set changed
bool Square::setPossible(int mk)
{
	bool changePossSet = false;
if (!(possibleSet&(1<<mk))) // mk is not already in possibility set
	{
		possibleSet=possibleSet|(1<<mk); // insert mk into possibility set
		changePossSet = true;
	}
	return changePossSet;
}

// prints the square to output stream in human readible form
ostream& Square::print(ostream& out) const
{
	int count = 0;
	// output mark:
	out << "Mark=";
	if (mark == -1)
	{
		out << "_";
	}
	else
	{
		out << mark;
	}
	// output immutability:
	if (immutable)
	{
		out << " (immutable); possibilities: ";
	}
	else
	{
		out << "            ; possibilities: ";
	}
	// output possibility set, using 1-9 and _:
	for (int bit = 0; bit <=9; bit++) 
	{
		if (count != 0) // get rid of extra bit
		{
			if (possibleSet & (1 << bit)) // if bit is 1, return bit number
			{
				out << bit;
			}
			else // if bit is 0, return _
			{
				out << "_";
			}
		}
		count++;
	}
	return out;
}

// writes square to output stream in a machine-readable form
ostream& Square::serialize(ostream& out) const
{
	// output mark:
	out << mark << " ";
	// output immutable:
	if (immutable)
	{
		out << 1 << " ";
	}
	else
	{
		out << 0 << " ";
	}
	//output possibility set assuming bit-vector representation:
	out << hex << possibleSet << dec;
	//cout << hex << possibleSet << dec;
	return out;
}

//de-serialize square from file input stream
ifstream& Square::realize(ifstream& file)
{
	//store mark, immutability and possibility set
	file >> mark;
	int i;
	file >> i;
	if (i == 1)
	{
		immutable = true;
	}
	else
	{
		immutable = false;
	}
	file >> hex >> possibleSet >> dec;
	return file;
}

//reinitialize implicit parameter by copying from explicit sq
//will not affect other BSquare members
void Square::reset(const Square& sq)
{
	mark = sq.mark;
	immutable = sq.immutable;
	possibleSet = sq.possibleSet;
}

//set mark and immutability
void Square::setInitialMark(int mk)
{
	mark = mk;
	if (mark != -1)
	{
		immutable = true;
	}
}

// returns a bool saying whether or not the square is blank
// (does not require caller to know how blank squares are encoded
bool Square::isBlank() const
{
	bool isBlank = false;
	if (Square::getMark() == -1)
	{
		isBlank = true;
	}
	return isBlank;
}

//returns true if the possibility set is non-empty
bool Square::somePossible() const
{
	bool somePossible = false;
	if (possibleSet != 0)
	{
		somePossible = true;
	}
	return somePossible;
}

//returns the possibility set of a square in string format
string Square::getPossible() const
{
	stringstream ss;
	int count = 0;
	//do not show possibility set if empty
	if (possibleSet == 0) 
	{
		ss<< "         ";
	}
	//translate possibility set into string
	else
	{
		for (int bit = 0; bit <=9; bit++) 
		{
			if (count != 0) // get rid of extra bit
			{
				if (possibleSet & (1 << bit)) // if bit is 1, return bit number
				{
					ss << bit;
				}
				else // if bit is 0, return _
				{
					ss << "_";
				}
			}
			count++;
		}
	}
	return ss.str();
}

//copy information from square s
void Square::copySquare(Square s)
{
	//copy each data member
	mark = s.mark;
	immutable = s.immutable;
	possibleSet = s.possibleSet;
}
