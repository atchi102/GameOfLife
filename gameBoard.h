#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class GameBoard{
public:
  int rows;
  int columns;
  int* currentBoard;
  int* nextBoard;
  int mode;
  int currentPopulation;
  int prevPopulation;
  int outputMode;
  string outputFile;
  int roundNum;
  GameBoard();
  ~GameBoard();
  void createBoard(int row, int column);
  int countNeighbors(int row, int column);
  void outputBoard();
  void populateSpace(int value, int r, int c);
  void assignSpace(int value, int r, int c);
  void setMode(int givenMode);
  void setOutputMode(int givenMode);
  int thisSpace(int r, int c);
  void nextGeneration();
  int neighborValue(int r, int c);
  void output(string printThis);

};
