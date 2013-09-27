/*
 * Square.hpp
 *
 *  Created on: Sep 9, 2012
 *      Author: Michael Fischer
 *  Modified on: Dec 7, 2012
 *  By: Rosie Buchanan
 */

#pragma once
#include "tools.hpp"

// Global parameters
const int minMark = 1;
const int maxMark = 9;
const int blankMark = -1;

class Square {
//give BoardState access, as it contains an array of squares
friend class BoardState;
private:
	int mark;				// current mark
	bool immutable;			// initial mark; can't be changed
	unsigned possibleSet;	// bit vector of possible marks

	void checkMark(int mk) const; // checks mk for validity
protected:
	//set mark and immutability
	void setInitialMark(int mk);

public:
	// construct a blank or initially marked square
	Square();

	// construct a square from an open input stream
	Square(istream& in);

	void setMark(int mk);
	int getMark() const {
		return mark;
	}

	bool isImmutable() const {
		return immutable;
	}

	bool isPossible(int mk) const;
	bool setPossible(int mk);
	bool unsetPossible(int mk);
	unsigned getPossibleSet() { return possibleSet; }

	ostream& print(ostream& out) const;
	ostream& serialize(ostream& out) const;
	
	//de-serialize square from file input stream
	ifstream& realize(ifstream& file);
	
	//reinitialize implicit parameter by copying from explicit sq
	//will not affect other BSquare members
	void reset(const Square& sq);
	// returns a bool saying whether or not the square is blank
	// (does not require caller to know how blank squares are encoded
	bool isBlank() const;
	//returns true if the possibility set is non-empty
	bool somePossible() const;
	//returns the possibility set of square in string format
	string getPossible() const;
	
	//initialize square to blank, with all marks possible
	void initializePossible() { possibleSet = 0x000003fe; }
	void initializeMark() { mark = -1; }
	void initializeImmutable() { immutable = false;}
	
	//copy information from square s
	void copySquare(Square s);

	//set possible set to zero; square is marked
	void clear() {possibleSet = 0;}
};

//output operator extension
inline ostream& operator<<(ostream& out, const Square& sq) {
	return sq.print(out);
}
