/*

Title: Helper2 (CPSC 427)
Date: 07-December-2012
File: BSquare.hpp
Author: Rosie Buchanan
Version: 6.0

Helper: Sudoku helper application that can be run by a user from the command line that will let him or her interactively solve a Sudoku puzzle. The program stores information for a sudoku board by storing information for the individual sudoku squares. Information stored for each square includes row and column position within the board, the possibility set of marks, the current mark, and whether or not the mark is immutable. Additional information is stored for the board itself: the number of rows, the number of columns, and the total number of squares. Clusters, a set of squares that must be filled in with distinct marks, has been introduced. This program handles three types of classical Sudoku clusters: rows, columns and squares. Each time a mark is recorded for a square, the mark is removed from the possibility sets of the the other squares in each of the clusters it is contained in. The board now prints the Board nicely using Unicode characters (U+2500 code chart). Users are able to use undo and redo functions, and add or erase individual marks.

BSquare: This extends the Square class by adding constant row and column data members as additional information stored for each square to indicate where square is located in the board. BSquare now extended with a list of back pointers to the Cluster objects of which it is a member.

*/

#include "tools.hpp"
#include "Square.hpp"
#include <vector>

//include guard
#ifndef BSQUARE_H
#define BSQUARE_H

class Cluster;

//BSquare extends Square
class BSquare : public Square {
private:
	int row; // row index (starting at 1) variable
	int column; // column (starting at 1) variable 
	vector<Cluster*> clu; // store list of ptrs to clusters of which this BSq is member
public:
	// print information about BSquare to output stream
	//modified to print names of clusters of which this BSq is member
	ostream& print(ostream& out) const;
	//initializes row and column data members of each BSquare
	void setCoordinates(int r, int c);
	//append argument clup to the cluster vector clu
	void addCluster(Cluster* clup);
	//creates and returns a string that uniquely identifies BSq
	string stringName() const;
	//calls its delegate setInitialMark() in the Square class and calls shoop()
	bool setInitialMark(int mk);
	//calls it delegate setMark() in class Square and then calls shoop()
	void setMark(int mk);
	//updates possibility set in all clusters to which it belongs if square is not blank
	bool shoop() const;
	//initialize sq to empty with any mark possible
	void initializeSquare();
};

//output operator extension
inline ostream& operator<<(ostream& out, const BSquare& sq) {
	return sq.print(out);
}

#endif
