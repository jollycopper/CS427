/*

Title: Helper2 (CPSC 427)
Date: 07-December-2012
File: Cluster.cpp
Author: Rosie Buchanan
Version: 2.0

Helper: Sudoku helper application that can be run by a user from the command line that will let him or her interactively solve a Sudoku puzzle. The program stores information for a sudoku board by storing information for the individual sudoku squares. Information stored for each square includes row and column position within the board, the possibility set of marks, the current mark, and whether or not the mark is immutable. Additional information is stored for the board itself: the number of rows, the number of columns, and the total number of squares. Clusters, a set of squares that must be filled in with distinct marks, has been introduced. This program handles three types of classical Sudoku clusters: rows, columns and squares. Each time a mark is recorded for a square, the mark is removed from the possibility sets of the the other squares in each of the clusters it is contained in. The board now prints the Board nicely using Unicode characters (U+2500 code chart). Users are able to use undo and redo functions, and add or erase individual marks.

Cluster: Represents a set of squares that must be filled in with distinct marks. For this assignment, cluster types are restricted to rows, columns and boxes.
*/
#include "tools.hpp"
#include "Cluster.hpp"

//print names of cluster type
const char* Cluster::cluPrintName[3] = {"Row", "Col", "Box"};

//creates Cluster object with the given type and ID
//initialize bsq[] array from pointers in its fourth argument
Cluster::Cluster(ClusterType typ, int id, int size, BSquare* bsqArg[])
{
	if ((size > maxCluSize) || (size < 1))
	{
		throw Fatal("Invalid cluster size!");
	}
	
	//copy arguments
	cluType = typ;
	clusterID = id;
	numBsq = size;

	int i = 0;
	//copy pointers
	for (i = 0; i < size; i++)
	{
		bsq[i] = bsqArg[i];
		bsq[i]->addCluster(this);
	}
}

//prints out line describing cluster
ostream& Cluster::print(ostream& out) const
{
	out << stringName() << ":";
	int i = 0;
	//print which BSquares are contained in cluster
	for (i = 0; i < numBsq; i++)
	{
		out << " " << bsq[i]->stringName();
	}
	return out;
}

//creates and returns a string that uniquely identifies the cluster
string Cluster::stringName() const
{
	stringstream stream;
	stream << cluPrintName[cluType] << " " << clusterID;
	return stream.str();
}

//performs a shoop operation on its cluster
//must not modify the possibility set of &bs
//unsets mark of bs from possibility sets of all other squares in the cluster
void Cluster::shoop(const BSquare* bs)
{
	int mark = bs->BSquare::getMark();
	//loop through squares pointed to by cluster
	for (int i = 0; i < maxCluSize; i++)
	{
		if (bsq[i] != bs && bsq[i] != NULL) //square is not the input bsq and isn't blank
		{
			bsq[i]->unsetPossible(mark); //unset possible mark
		}
	}
}
