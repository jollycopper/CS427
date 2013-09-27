Title: Helper2 (CPSC 427)
Date: 07-December-2012
Author: Rosie Buchanan
Version: 1.0

Files included in submission: Square.cpp, Square.hpp, BSquare.cpp, BSquare.hpp, Board.cpp, Board.hpp, Clusters.hpp, Clusters.cpp, BoardViewer.cpp, BoardViewer.hpp BoardStack.cpp, BoardStack.hpp, BoardState.cpp, BoardState.hpp, Game.cpp, Game.hpp tools.cpp, tools.hpp, Makefile, README.txt, SampleOutput.txt

Overview: Sudoku helper application that can be run by a user from the command line that will let him or her interactively solve a Sudoku puzzle. The program stores information for a sudoku board by storing information for the individual sudoku squares. Information stored for each square includes row and column position within the board, the possibility set of marks, the current mark, and whether or not the mark is immutable. Additional information is stored for the board itself: the number of rows, the number of columns, and the total number of squares. Clusters, a set of squares that must be filled in with distinct marks, has been introduced. This program handles three types of classical Sudoku clusters: rows, columns and squares. Each time a mark is recorded for a square, the mark is removed from the possibility sets of the the other squares in each of the clusters it is contained in. The board now prints the Board nicely using Unicode characters (U+2500 code chart). Users are able to use undo and redo functions, and add or erase individual marks, as well as load previously saved games or save current games for later play. The saved games include the undo/redo stacks and current state. The games are saved in a serialized form and de-serialized upon load.

BSquare.cpp/.hpp: extends class Square from previous problem set by adding constant row and column members to indicate where the square is located on the board. Each BSquare - extended square - stores possibility set, mark, and immutability as well.
Functionality added: initializeSquare() to create blank, unmarked square with all possibilities allowed

Board.cpp/.hpp: Represents a Sudoku board. It has constant data members that contain the number of rows, the number of columns, and the total number of squares. It also has a constant data member pointer that points to a dynamically allocated array of BSquare. Now, the file reading and storage allocation take place separately so that no memory leaks occur with invalid filenames. The rest of the code/files were fixed for this adjustment. Board now creates all the clusters in the board and maintains a master list of all Cluster objects. It can print out all clusters in the board and delete the clusters when the board is deleted.
Functionality added: get functions to access data members of BSquares, board initializer to create blank board of unmarked squares
with all possibilities allowed, mark setter to set a mark from the board, BSquare copier to copy a given square to a certain square on the board

Cluster.cpp/hpp: Cluster class represents set of squares that must be filled in with distinct marks. Here, the program handles row, column, and box type clusters. Each Cluster stores type, ID, number of BSquares, and an array of pointers to the BSquares that belong to the clusters.

Shoop aspect (in BSquare / Cluster): Each time a mark is recorded in a square, the program looks at the clusters that contains the square, and within those clusters, it deletes the mark from the possibility sets of all other squares (not including original square). This allows the program to determine if a given board is impossible, and alert the user when an impossible mark is attempted.

BoardViewer: Derived from Viewer that provides implementations for the pure virtual functions in the base class. The BoardViewer class is given access to a Board object in order to obtain the data needed by the viewer. The board is then printed using Unicode characters to draw the thick and thin grid lines commonly used in drawing a Sudoku board. Each Sudoku square is represented by a 4 × 9 array of characters. The top row is used to display the possibility set. The third row is used to display the mark.

BoardState.cpp/hpp: A copiable class used to store the current state of the board. This consists of copies of the Square objects in an array that comprise the board but not the clusters and other information that is in BSquare (and which doesn’t change once the board has been constructed). A BoardState can be created from a board, or initialized to a clean BoardState. Data members are dimensions and the array of squares.

BoardStack.cpp/hpp: A stack of BoardState pointers used to implement the undo and redo stacks. It is derived vector<BoardState⋆> and should support the usual stack operations as well as a flush operation that empties out the stack. It is derived from vector, but hides unneccessary public functions. Includes push,pop,peek,etc. functions associated with stacks.

Game.cpp/hpp: The top-level class that runs the game. It owns the board, the undo and redo stacks, and the board viewer. It implements the main command loop and the code for the individual commands, and each command is implemented by a separate member function, as well as the command dispatcher (the code that reads the command letter and calls the appropriate command function). This runs the game by creating a board, the associated board states, and controlling pushes and pops to the stacks. The bulk of the game functions are included here. User can load both puzzles and saved saves, and save current games in a serialized form. Game.cpp handles both serialization and de-serialization by delegating to BoardStack, BoardState, and Square. Loading a file allows the user to change the dimensions of the game. Loading fails when there is an error reading in the serialized form, there are extra characters in the file, or the game is not Classical Sudoku.

main.cpp contains the test script to test the BSquare, Board, Cluster, BoardViewer, BoardState, BoardStack, and Game classes/functions. 

To run program, 2 arguments are needed:
1. Number of rows in board
2. Number of columns in board

To compile: make helper2
To run (example): ./helper2 9 9

See file SampleOutput.txt for sample.
