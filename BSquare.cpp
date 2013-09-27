/*

Title: Helper2 (CPSC 427)
Date: 07-December-2012
File: BSquare.cpp
Author: Rosie Buchanan
Version: 6.0

Helper: Sudoku helper application that can be run by a user from the command line that will let him or her interactively solve a Sudoku puzzle. The program stores information for a sudoku board by storing information for the individual sudoku squares. Information stored for each square includes row and column position within the board, the possibility set of marks, the current mark, and whether or not the mark is immutable. Additional information is stored for the board itself: the number of rows, the number of columns, and the total number of squares. Clusters, a set of squares that must be filled in with distinct marks, has been introduced. This program handles three types of classical Sudoku clusters: rows, columns and squares. Each time a mark is recorded for a square, the mark is removed from the possibility sets of the the other squares in each of the clusters it is contained in. The board now prints the Board nicely using Unicode characters (U+2500 code chart). Users are able to use undo and redo functions, and add or erase individual marks.

BSquare: This extends the Square class by adding constant row and column data members as additional information stored for each square to indicate where square is located in the board. BSquare now extended with a list of back pointers to the Cluster objects of which it is a member.

*/

#include "BSquare.hpp"
#include "Cluster.hpp"


//print information about BSquare in a human-readable form, now including cluster info
ostream& BSquare::print(ostream& out) const
{
	out << "BSquare " << stringName() << ": in "; // print BSquare position
	stringstream stream;
	stream << clu[0]->stringName();
		
		//print cluster membership info
		unsigned int i = 1;
		for (i = 1; i < Cluster::numClusterType; i++)
		{
			stream << ", " << clu[i]->stringName();
		}
		stream << "; ";
		out << stream.str();
	Square::print(out); // print information stored in BSquare using Square class print
	return out;
} 

//initializes row and column data members of each BSquare
void BSquare::setCoordinates(int r, int c)
{
	row = r;
	column = c;
}

//add its cluster pointer argument to its list of back pointers to Cluster objects of which BSquare is a member
//append argument to cluster vector
void BSquare::addCluster(Cluster* clup)
{
	clu.push_back(clup);
}

//creates and returns a string that uniquely identifies this BSquare
//form: (row, column)
string BSquare::stringName() const
{
	stringstream stream;
	stream << "(" << row << ", " << column << ")";
	return stream.str();
}
//calls its delegate setInitialMark() in the Square class and calls shoop()
bool BSquare::setInitialMark(int mk)
{
	Square::setInitialMark(mk);
	return BSquare::shoop();
}

//calls it delegate setMark() in class Square and then calls shoop()
void BSquare::setMark(int mk)
{
	Square::setMark(mk);
	BSquare::shoop();
	Square::clear();
}
//does nothing on blank squares
//otherwise, checks that the square's mark is in its possibility set and throws error if not
//updates possibility set in all clusters to which it belongs by calling Cluster::shoop
bool BSquare::shoop() const
{
	bool success = true;
	int mk = getMark();
	if (isBlank())
	{
		//do nothing for blank square
	}
	else
	{
		if (!isPossible(mk)) //throw error, mark not possible
		{
			cout << "Mark " << mk << " not possible in square " << stringName(); 
			cout <<endl;
			success = false;
		}		
		else
		{
			//loop through the clusters the bsquare is part of
			//if cluster isn't null, shoop the cluster using the bsquare
			for (int i = 0; i < Cluster::numClusterType; i++)
			{
				if (clu[i] != NULL)
				{
					clu[i]->Cluster::shoop(this);
				}
			}
		}

	}
	return success;
}
//initialize sq to empty with any mark possible
void BSquare::initializeSquare()
{
	initializeMark();
	initializePossible();
	initializeImmutable();
}
