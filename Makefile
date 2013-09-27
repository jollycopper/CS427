# Author: Rosie Buchanan
# Date: 07 December 2012
# CS 427: Problem Set #9

CXXFLAGS = -O1 -g -Wall
OBJ = main.o BoardStack.o BoardState.o Game.o BoardViewer.o Cluster.o Board.o BSquare.o Square.o tools.o libviewer.a
OBJ2 = main.o BoardStack.o BoardState.o Game.o BoardViewer.o Cluster.o Board.o BSquare.o Square.o tools.o
TARGET = helper2

$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ)

clean:
	rm -f $(OBJ2) $(TARGET)

