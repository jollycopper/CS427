/*

Title: Helper2 (CPSC 427)
Date: 07-December-2012
File: Cluster.hpp
Author: Rosie Buchanan
Version: 2.0

Helper: Sudoku helper application that can be run by a user from the command line that will let him or her interactively solve a Sudoku puzzle. The program stores information for a sudoku board by storing information for the individual sudoku squares. Information stored for each square includes row and column position within the board, the possibility set of marks, the current mark, and whether or not the mark is immutable. Additional information is stored for the board itself: the number of rows, the number of columns, and the total number of squares. Clusters, a set of squares that must be filled in with distinct marks, has been introduced. This program handles three types of classical Sudoku clusters: rows, columns and squares. Each time a mark is recorded for a square, the mark is removed from the possibility sets of the the other squares in each of the clusters it is contained in. The board now prints the Board nicely using Unicode characters (U+2500 code chart). Users are able to use undo and redo functions, and add or erase individual marks.

Cluster: Represents a set of squares that must be filled in with distinct marks. For this assignment, cluster types are restricted to rows, columns and boxes.

*/

#pragma once
#include "tools.hpp"
#include "Board.hpp"

//global parameter
const int maxCluSize = maxMark-minMark+1; //max num squares permitted in any cluster

class Cluster {

private:
	int clusterID; //unique cluster ID
	int numBsq; //total number of BSquares in cluster
	BSquare* bsq[maxCluSize]; //array of pointers to the BSquares that belong to the cluster
public:
	enum ClusterType { ROW, COL, BOX, numClusterType }; //define clusterTypes
	ClusterType cluType; //cluster type
	//print names of the cluster types
	static const char* cluPrintName[3];

	//create new Cluster object with the given ClusterType typ and ID id
	Cluster(ClusterType typ, int id, int size, BSquare* bsqArg[]);

	//prints line describing cluster
	ostream& print(ostream& out) const;

	//creates and returns a string that uniquely identifies cluster
	//builds string from typ and id fields passed to constructer used to create cluster
	string stringName() const;
	//performs a shoop operation on its cluster
	//must not modify the possibility set of &bs
	void shoop(const BSquare* bs);
};

//output operator extension
inline ostream& operator<<(ostream& out, const Cluster& clu) {
	return clu.print(out);
}
