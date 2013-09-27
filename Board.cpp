/*

Title: Helper2 (CPSC 427)
Date: 07-December-2012
File: Board.cpp
Author: Rosie Buchanan
Version: 7.0

Helper: Sudoku helper application that can be run by a user from the command line that will let him or her interactively solve a Sudoku puzzle. The program stores information for a sudoku board by storing information for the individual sudoku squares. Information stored for each square includes row and column position within the board, the possibility set of marks, the current mark, and whether or not the mark is immutable. Additional information is stored for the board itself: the number of rows, the number of columns, and the total number of squares. Clusters, a set of squares that must be filled in with distinct marks, has been introduced. This program handles three types of classical Sudoku clusters: rows, columns and squares. Each time a mark is recorded for a square, the mark is removed from the possibility sets of the the other squares in each of the clusters it is contained in. The board now prints the Board nicely using Unicode characters (U+2500 code chart). Users are able to use undo and redo functions, and add or erase individual marks.

Board: Represents a Sudoku board with constant data members that contain the number of rows, number of columns, and the total number of squares. It has a constant data member pointer that points to a dynamically allocated array of BSquares. Board maintains a master list of all cluster objects. 

*/

#include "tools.hpp"
#include "Cluster.hpp"

//reads board from file, stores marks
//if invalid file, no memory leak
bool Board::readMarks(string boardFile)
{
	bool success = true;
	ifstream inFile(boardFile.c_str()); //open stringname as file
	//check valid file	
	if (!inFile)
	{
		cout << "Error: Can't open board file " << boardFile;
		success = false;
	}
	else
	{
		int currRow = 1; // reading row index
		int currCol = 1; // reading col index

		char store; // for reading in file chars
		int storeInt = 0; // for storing corresponding ints 

		int subsc = 0;

		while(!inFile.eof())
		{
			inFile >> store;
			//check for blank mark and store appropriately
			if (store == '-')
			{
				storeInt = -1;
			}
			//store mark as int
			else
			{
				storeInt = (store - '0');
			}
			//calculate linear subscript array position
			subsc = sub(currRow, currCol);
			//set initial mark
			//if (storeInt != -1) {brd[subsc].BSquare::clear(); }
			success = brd[subsc].BSquare::setInitialMark(storeInt);
			if (success == false) {return success;}
			//stop reading after last square is read
			if ((currRow == rows) && (currCol == cols)) {break;}
			//move on to next row once encounter last square in column
			if (currCol == cols)
			{
				currCol = 1;
				currRow++;
			}
			//move on to square in next column if currCol is not the last one
			else
			{
				currCol++;
			}
		}
		if (!isStillPossible())
		{
			cout << "Board is not solvable" << endl;
			success = false;
			return success;
		}
		else
		{
			clearPossSets();
		}
	}
	return success;
}

//construct Board with nr rows, nc columns, assigns correct row and column indices
//memory leaks now impossible
Board::Board(int nr, int nc): rows(nr), cols(nc), total(nr*nc)
{
	brd = (new BSquare[nr*nc]);	
	int i = 0;
	int currRow = 1; // reading row index
	int currCol = 1; // reading col index

	while (i < (nr*nc))
	{
		brd[i].BSquare::setCoordinates(currRow, currCol);
		if (currCol == nc)
		{
			currRow++;
			currCol = 1;
		}
		else
		{
			currCol++;
		}
		i++;
	}
	//create clusters
	makeRowClusters();
	makeColClusters();
	makeBoxClusters();
}

//calculate linear subscript between 0 and (total-1) for Bsquare in position (r,c)
int Board::sub(int r, int c) const
{
	int rowMultiplier = (r-1)*cols; //account for row number and 0-indexing
	int subscript = rowMultiplier+(c-1);
	return subscript;
}

// writes current board marking to stream out in the same format as is used by the Board constructor when creating the Board
ostream& Board::writeMarks(ostream& out) const
{
	out << "Writing the marks:" << endl;
	int currRow = 1; // current column index
	int currCol = 1; // current row index
	int subsc = 0; // current square index of Board
	
	while (currRow <= rows && currCol <= cols) // still inside bounds of Board
	{
		// test for blank mark by using getMark() method in Square class	
		if (brd[subsc].Square::getMark() == -1)
		{
			out << "-"; // output symbol for blank
		}
		// square is not blank
		else
		{
			out << brd[subsc].Square::getMark(); // output mark
		}
		// test for reaching end of row and move on to beginning of next row
		if (currCol == cols)
		{
			out << "\n";
			currCol = 1;
			currRow++;
		}
		else
		{
			currCol++; // move to next square
		}
	subsc++; // increment Board square
	}
	return out;
}

// print function that prints the dimensions of the board of the first line
// followed by one line for each square of the board
ostream& Board::print(ostream& out) const
{
	for (int i = 0; i < total; i++) // loops through all the squares
	{
		brd[i].BSquare::print(out); // print information for each Square
		out << endl; // move to next line
	}
	//print cluster information
	// one line per cluster containing the coordinates of squares included
	out << "Clusters:";
	for (unsigned int i = 0; i < clu.size(); i++)
	{
		out << "\n" << *clu[i];
	}
	out << "\n" << endl;
	return out;
}

//store list of all row clusters
void Board::makeRowClusters()
{
	BSquare* temp[9];
	int i = 0;
	int j = 0;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			temp[j] = &brd[sub(i+1,j+1)]; //store in temp array
		}
		//create new cluster from temp
		clu.push_back(new Cluster(Cluster::ROW, i+1, cols, temp));
	}
}

//store list of all column clusters
void Board::makeColClusters()
{
	BSquare* temp[9];
	int i = 0;
	int j = 0;
	for (i = 0; i < cols; i++)
	{
		for (j = 0; j < rows; j++)
		{
			temp[j] = &brd[sub(j+1,i+1)]; //store in temp array
		}
		//create new cluster from temp
		clu.push_back(new Cluster(Cluster::COL, i+1, rows, temp));
	}
}

//store list of all box clusteres
void Board::makeBoxClusters()
{
	BSquare* temp[9];
	//determine number of horizontal box clusters
	int numHorClu;
	if (rows % 3 == 0)
	{
		numHorClu = rows/3;
	}
	else
	{
		numHorClu = (rows/3) + 1;
	}
	//determine number of vertical box clusters
	int numVerClu;
	if (cols % 3 == 0)
	{
		numVerClu = cols/3;
	}
	else
	{
		numVerClu = (cols/3) + 1;
	}
	
	//loop indices
	int i = 0;
	int j = 0;
	int k;
	int l;
	int count = 0;
	//loop through clusters
	for (i = 0; i < numHorClu; i++)
	{
		for (j = 0; j < numVerClu; j++)
		{
			count = 0; //counter
			//loop through squares in cluster
			for (k = 1+(3*i); k < 1+(3*(i+1)) && k <= rows; k++)
			{
				for (l = 1+(3*j); l < 1+(3*(j+1)) && l <= cols; l++)
				{
					temp[count] = &brd[sub(k,l)]; //store in temp array
					count++;
				}
			}
			//create new cluster from temp
			clu.push_back(new Cluster(Cluster::BOX, 1+(3*i)+j, count, temp));
		}
	}
}

//delete clusters
Board::~Board() 
{
	//delete board
	delete[] brd;
	unsigned int i = 0;
	//delete individual clusters
	for (i = 0; i<clu.size(); i++)
	{
		delete clu[i];
	}
}

//returns true if all unmarked squares have non-empty possibility lists
bool Board::isStillPossible() const
{
	bool isStillPossible = true;

	for (int i = 0; i < total; i++) // loops through all the squares
	{
		if (brd[i].BSquare::isBlank()) //blank square
		{
			if (brd[i].BSquare::somePossible() != true) //no possibilities
			{
				isStillPossible = false;
			}
	
		}
	}
	return isStillPossible;
}

// returns mark at given row and column of board
int Board::boardGetMark(int row, int col) const
{
	int sub = Board::sub(row, col); //calculate index
	return brd[sub].BSquare::getMark();
}

//returns possibility set of given row and column of board
string Board::boardGetPossString(int row, int col) const
{
	int sub = Board::sub(row, col); //calculate index
	string possString = brd[sub].BSquare::getPossible();
	return possString;
}

//unset possible mk from square at (row, col)
void Board::boardUnsetPoss(int row, int col, int mark)
{
	int sub = Board::sub(row, col); //calculate index
	brd[sub].BSquare::unsetPossible(mark);
}

//initializes board to empty, with all marks possible
void Board::initializeBoard()
{
	//initialze every square of board
	for (int i = 0; i < total; i++)
	{
		brd[i].initializeSquare();
	}
}

//sets mark m at (r,c) and returns bool representing successfulness
bool Board::boardSetMark(int r, int c, int m)
{
	bool markSet = false;
	int index = sub(r, c); //calculate index
	//square already marked
	if (brd[index].getMark() != -1)
	{
		cout << "Error: Attempt to mark already-marked square" << endl;
	}	
	//square can be marked, mark square
	else if (brd[index].BSquare::isPossible(m))
	{
		brd[index].setMark(m);
		markSet = true;
	}
	//mark not in possibility set
	else
	{
		cout << "Mark " << m << " not possible in square " << brd[index].stringName() << endl;
	}
	return markSet;
}

//copies square s to board at given index
void Board::copySquare(int index, Square s)
{
	brd[index].Square::copySquare(s);
}

//clears the possibility sets for marked squares of the board
Square Board::getSquare(int i)
{
	return brd[i];
}

//clears the possibility sets for marked squares of the board
void Board::clearPossSets()
	{
		for (int i = 0; i < total; i++)
		{
			if (brd[i].getMark() != -1)
			{
				brd[i].clear();
			}
		}
	}
