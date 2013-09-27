/*
Title: Helper2 (CPSC 427)
Date: 07-December-2012
File: Game.cpp
Author: Rosie Buchanan
Version: 2.0

Helper: Sudoku helper application that can be run by a user from the command line that will let him or her interactively solve a Sudoku puzzle. The program stores information for a sudoku board by storing information for the individual sudoku squares. Information stored for each square includes row and column position within the board, the possibility set of marks, the current mark, and whether or not the mark is immutable. Additional information is stored for the board itself: the number of rows, the number of columns, and the total number of squares. Clusters, a set of squares that must be filled in with distinct marks, has been introduced. This program handles three types of classical Sudoku clusters: rows, columns and squares. Each time a mark is recorded for a square, the mark is removed from the possibility sets of the the other squares in each of the clusters it is contained in. The board now prints the Board nicely using Unicode characters (U+2500 code chart). Users are able to use undo and redo functions, and add or erase individual marks.

Game: The top-level class that runs the game. It owns the board, the undo and redo stacks, and the board viewer. It implements the main command loop and the code for the individual commands, and each command is implemented by a separate member function, as well as the command dispatcher (the code that reads the command letter and calls the appropriate command function).

*/

#include "Game.hpp"

// create game of dimensions (r, c)
Game::Game(int r, int c):rows(r),cols(c) {}

//print welcome msg
void Game::welcome()
{
	cout << "Welcome to Sudoku Helper!" << endl;
}

//print menu of possible commands
void Game::menu()
{
	cout << "Command lines are single letters followed by arguments." << endl;
	cout << "Whitespace is ignored everywhere except within file names." << endl;
	cout << "Commands are:" << endl;
	cout << "  l <file>\t- Loads a puzzle or saved game from a file" << endl;
	cout << "  s <file>\t- Save a game to a file" << endl;
	cout << "  r <rcm>\t- Removes a possibility" << endl;
	cout << "  m <rcm>\t- Marks a square" << endl;
	cout << "  z\t\t- Undo" << endl;
	cout << "  y\t\t- Redo" << endl;
	cout << "  q\t\t- Quit" << endl;
	cout << "  ?\t\t- Show these instructions" << endl;
	cout << "  (empty)\t- Redisplay the board" << endl;
	cout << "Arguments:" << endl;
	cout << "  <file> is the name of a puzzle file." << endl;
	cout << "  <rcm> is three digits specifying a row, column, and mark." << endl;
}

//command prompt
void Game::askCommand()
{
	cout << "Enter command:  l <file>; s <file>; r <rcm>; m <rcm>; z; y; q; ?; (empty)" << endl;
	cout << "> ";
}

//create and initialize board with no marks, full possibility sets, and mutable squares 
Board* Game::createBoard()
{
	Board* b = new Board(rows, cols); //allocate storage for board
	b->Board::initializeBoard(); //initialize board to all marks possible for each square
	return b;
}

//utilize BoardViewer to display board in nice format
void Game::showBoard(Board* b)
{
	BoardViewer bv = BoardViewer(b);
	bv.BoardViewer::show(cout);
}

//de-serialize game from input stream file; return Game* object that resulted from file read
Game* Game::realize(Board* b, ifstream& file, string filename)
{
	Game* newGame = NULL;
	//analyze game descriptor
	string gameDescriptor = "";
	getline(file, gameDescriptor);
	if (gameDescriptor != "Classical Sudoku")
	{
		cout << "Error: Can't load game type " << gameDescriptor << endl;
		newGame = NULL;
	}
	//descriptor is good
	else
	{
	//store rows and columns
		int trows;
		int tcols;
		file >> trows;
		file >> tcols;
		newGame = new Game(trows, tcols);
		//de-serialize information into the 2 stacks
		try 
		{
			newGame->undo.BoardStack::realize(rows, cols, file);
			newGame->redo.BoardStack::realize(rows, cols, file);
			//current board state is on top
			BoardState* curr = newGame->undo.BoardStack::peek();
			//update board from boardstate
			curr->BoardState::fromBoardState(b);
		}
		//delete if de-serializing fails
		catch(ifstream::failure e)
		{
			
			newGame->undo.flush();
			newGame->redo.flush();
			delete newGame;
			throw;
		}
		ws(file);
		if (!file.eof())
		{
			cout << "Error: Unread garage in load file" << endl;
			newGame->undo.BoardStack::flush();
			newGame->redo.BoardStack::flush();
			delete newGame;
			return NULL;
		}
	}
	return newGame;
}


//load board from file
Game* Game::load(Board* b)
{
	Game* game;
	//get filename
	string filename;
	ws(cin);
	getline(cin, filename);
	ifstream inFile(filename.c_str());
	//is filename valid puzzle file?
	if (inFile && ((filename.length() < 4 || !(filename.substr(filename.length()-4, 4) == ".ssg"))))
	{
		//initialize board
		b->Board::initializeBoard();
		//store marks to board
		bool success = b->Board::readMarks(filename);
		//store board info in board state
		BoardState* toPush;
		//store boardstate on undo stack
		toPush = new BoardState(b);
		undo.BoardStack::push(toPush);
		//display board if successful store
		if (success)
		{
			showBoard(b);
			flushRedo();
		}
		//otherwise, return to state before file read
		else
		{
			undo.BoardStack::pop();
			redo.BoardStack::pop();
		}
		return NULL;
	}
	//is file valid .ssg file?
	else if (inFile)
	{
		//de-serialize the game
		game = realize(b, inFile, filename);
		//game successfully de-serialized
		if (game != NULL)
		{
			cout << "Saved game '" << filename <<"' loaded. Press Enter to continue." << endl;
			cin.get();
			return game;
		}
	}
	//is file valid .ssg with forgotten extension?
	else
	{
		//append extension
		string origFile = filename;
		filename.append(".ssg");
		ifstream inFile2(filename.c_str());
		if (inFile2)
		{
			//exception mask for readable files
			try
			{
				inFile2.exceptions(ifstream::failbit | ifstream::badbit);
				//de-serialize
				game = realize(b, inFile2, filename);
				//successful de-serialization
				if (game != NULL)
				{
					cout << "Saved game '" << filename <<"' loaded. Press Enter to continue." << endl;
					cin.get();
					return game;
				}
			}
			//file not formatted correctly
			catch(ifstream::failure e)
			{
				cout << "Error restoring game from file '" << filename << "'" << endl;
				return NULL;
			}
		}
		//invalid file
		else
		{
			cout << "Error: Cannot find either '" << origFile << "' or '" << filename << "'" << endl;
			return NULL;
		}
	}
	//return game from file, or null game if errors occurred
	return NULL;
}

//get row, column, and value of mark to be removed and remove mark from board, updating square info
void Game::remove(Board* b)
{
	bool badInput = false; //bool for keeping track of input validity

	//get row, column, mark
	char rowChar = ' ';
	char colChar = ' ';
	char mkChar = ' ';
	int row = 0;
	int col = 0;
	int mk = 0;
	//get line with r, c, m
	string line;
	getline(cin, line);
	//check all arguments entered
	istringstream iss(line);
	if (!(iss >> rowChar) || !(iss >> colChar) || !(iss >> mkChar))
	{
		cout << "Error reading mark command args" << endl;
		badInput = true;
	}
	else
	{
		//r, c, m chars to ints
		row = readRCM(rowChar);
		col = readRCM(colChar);
		mk = readRCM(mkChar);
		// check each within range
		badInput = checkRange(row,col,mk);
	}
	//all arguments entered and within range
	if (!badInput)
	{
		//unset mark from possibility set
		b->boardUnsetPoss(row, col, mk);
		//store board info to new board state on undo stack
		BoardState* toPush;
		toPush = new BoardState(b);
		undo.BoardStack::push(toPush);
		//show board
		showBoard(b);
	}
}

//change row/col/mark char into integer
int Game::readRCM(char c)
{
	int i;
	i = (int) (c - 48);
	return i;
}

//get row, column, and value of mark to be added and add mark from board, updating square info
void Game::mark(Board* b)
{
	bool badInput = false; //bool for keeping track of input validity
	//get row, column, mark
	char rowChar = ' ';
	char colChar = ' ';
	char mkChar = ' ';
	int row = 0;
	int col = 0;
	int mk = 0;
	//get line with r, c, m
	string line;
	getline(cin, line);
	//check all arguments entered
	istringstream iss(line);
	if (!(iss >> rowChar) || !(iss >> colChar) || !(iss >> mkChar))
	{
		cout << "Error reading mark command args" << endl;
		badInput = true;
	}
	else
	{
		//r, c, m chars to ints
		row = readRCM(rowChar);
		col = readRCM(colChar);
		mk = readRCM(mkChar);
		// check each within range
		badInput = checkRange(row,col,mk);
	}
	//all arguments entered and within range
	if (!badInput)
	{
		//attempts to set mark, returns true if successful
		//returns false if unsuccessful, prints error
		if (b->boardSetMark(row, col, mk))
		{
			//store board info to new board state on undo stack
			BoardState* toPush;
			toPush = new BoardState(b);
			undo.BoardStack::push(toPush);
			//show board
			showBoard(b);
		}
	}
}

//undo and return board to state of last move
void Game::undoF(Board* b)
{
	//pop current boardstate from undo
	BoardState* toRedo = undo.BoardStack::pop();
	//no state on stack other than current
	if (undo.BoardStack::empty())
	{
		cout << "Error: Can't undo" << endl;
		//put current state back on stack
		undo.BoardStack::push(toRedo);
	}
	else
	{
		//push current state to redo stack
		redo.BoardStack::push(toRedo);
		//update current state to last state
		BoardState* curr = undo.BoardStack::peek();
		//update board from boardstate
		curr->BoardState::fromBoardState(b);
		//show board
		showBoard(b);
	}
}

//redo and return board to state before undo command was called
void Game::redoF(Board* b)
{
	//no possible state to redo
	if (redo.BoardStack::empty())
	{
		cout << "Error: Can't redo" << endl;
	}
	else
	{
		//pop state from redo stack, push to undo stack;
		BoardState* toUndo = redo.BoardStack::pop();
		undo.BoardStack::push(toUndo);
		//update board from current state
		toUndo->BoardState::fromBoardState(b);
		//show board
		showBoard(b);
	}
}

//empty redo stack
void Game::flushRedo()
{
	if (!(redo.BoardStack::empty()))
	{
		redo.flush();
	}
}
//check range of rows/cols/marks
bool Game::checkRange(int r, int c, int m)
{
	bool outOfRange = false;
	if (r <= 0 || r > rows || c <= 0 || c > cols || m <= 0 || m > 9)
	{
		cout << "Error: Row, column, or mark out of range" << endl;
		outOfRange = true;
	}
	return outOfRange;
}

//quit and destroy game
void Game::quit(Board* b)
{
		//delete stacks
		redo.flush();
		undo.flush();
		//delete board
		delete b;
}

//control center for game: prompt for and follow commands; returns game object when user quits of new file is read correctly
Game* Game::runGame()
{
	//initialize command char
	char c = '\n';
	//initialize game and board
	Game* currGame = this;
	Board* b = createBoard();
	//print welcome and menu
	welcome();
	menu();
	//initialize bool for keeping track of whether this is first true game
	bool continueFlag = true;
	//undo stack is empty - first true game
	if (currGame->undo.BoardStack::empty())
	{
		//create first board state from default board, store on undo stack
		BoardState toPush = BoardState(rows, cols);
		currGame->undo.BoardStack::push(&toPush);
	}
	//game has been loaded from file
	else
	{
		//update current state to last state
		BoardState* curr = currGame->undo.BoardStack::peek();
		//update board from boardstate
		curr->BoardState::fromBoardState(b);
		continueFlag = false;
	}
	//bool for continuous play
	bool play = true;
	
	//continue getting commands until user quits
	while (play)
	{
		//first time played, get command
		if (continueFlag)
		{
			currGame->askCommand();
			cin.get(c);
		}
		//ignore spaces
		while (c == ' ')
		{
			cin.get(c);
		}
		//empty command
		if (c == '\n')
		{
			currGame->showBoard(b);
		}
		//load board from file
		else if (c == 'l')
		{
			Game* load = currGame->load(b);
			//file correctly loaded
			if (load != NULL)
			{
				delete b;
				play = false;
				return load;
			}
		}
		//remove mark from board, delete redo stack
		else if (c == 'r')
		{
			currGame->remove(b);
			currGame->flushRedo();
		}
		//add mark to board, delete redo stack
		else if (c == 'm')
		{
			currGame->mark(b);
			currGame->flushRedo();
		}
		//undo last move
		else if (c == 'z')
		{
			currGame->undoF(b);
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //get next command
		}
		//redo last undo
		else if (c == 'y')
		{
			currGame->redoF(b);
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //get next command
		}
		//print menu
		else if (c == '?')
		{
			menu();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //get next command
		}
		else if (c == 's')
		{
			currGame->save();
			//cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //get next command
		}
		//quit and destroy game
		else if (c == 'q')
		{
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //get next command
			//double check for quit
			cout << "Do you really want to quit? (y/N): ";
			char temp;
			if ((temp = cin.get()) == 'y')
			{
				//quit and delete game
				currGame->quit(b);
				currGame = NULL;
				play = false;
				//break;
				return currGame;
			}
			else if (temp != '\n')
			{
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //get next command	
			}
		}
		//invalid command
		else
		{
			cout << "Error: Unknown command" << endl;
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //get next command
		}
	//continue game unless new game was loaded
	continueFlag = true;
	}
	return currGame;
}

//run game!
void Game::run(int nr, int nc)
{
	//create new game
	Game* g = new Game(nr, nc);
	Game* temp;
	cout << "Starting the game:" << endl;
	cout << endl;
	temp = g->runGame();
	//play game
	while (temp != NULL)
	{
		g->redo.flush();
		g->undo.flush();
		delete g;
		g = temp;
		temp = g->runGame();
	}
	g->redo.flush();
	g->undo.flush();
	delete g;
}

//save game to .ssg file by serializing
void Game::save()
{
	//initialize bool for overwriting existing file
	bool overwrite = true;
	//get desired filename
	string line = "";
	getline(cin, line);
	string ext = ".ssg";
	//check is file extension needs to be appended
	stringstream iss(line);
	if (!(iss >> line)) {line = ext;}
	if (line.length() < 4 || !(line.substr(line.length()-4, 4) == ".ssg"))
	{
		line.append(ext);
	}
	//check if file exists
	FILE* f = fopen(line.c_str(),"r");
	if (f)
	{
		//check for overwrite
		cout << "File '" << line << "' exists. Do you want to overwrite it? (y/N): ";
		//overwrite denied
		char temp;
		//overwrite
		if ((temp = cin.get()) == 'y')
		{
			overwrite = true;
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //get next command
		}
		//nothing entered, abort
		else if (temp!= '\n')
		{
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //get next command	
			cout << "Error: Save aborted" << endl;
			overwrite = false;
		}
		//something other than yes entered, abort
		else
		{
			cout << "Error: Save aborted" << endl;
			overwrite = false;
		}
	}
	//overwrite permitted
	if (overwrite == true)
	{
		//output stream, output game descriptor and rows/cols
		ofstream myfile(line.c_str());
		myfile << "Classical Sudoku" << endl;
		myfile << rows << " " << cols << endl;
		//serialize game to file
		serialize(myfile);
		
		//close file and print confirmation
		cout << "Game saved to file '" << line << "'" << endl;
		myfile.close();
	}
}

//serialize game to output stream
ostream& Game::serialize(ostream& out)
{
	//serialize non-empty stacks
	if (!undo.BoardStack::empty())
	{
		undo.BoardStack::serialize(out);
	}
	if (!redo.BoardStack::empty())
	{
		redo.BoardStack::serialize(out);
	}
	return out;
}


