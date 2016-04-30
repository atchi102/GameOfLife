#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "gameBoard.h"

using namespace std;



GameBoard::GameBoard()
{
}
//clear dynamically allocated memory
GameBoard::~GameBoard()
{
  delete currentBoard;
  delete nextBoard;
}
//create two game board and set variables
void GameBoard::createBoard(int row, int column)
{
  currentBoard = new int[row*column];
  nextBoard = new int[row*column];

  rows = row;
  columns = column;
  currentPopulation = -1; //set population to -1 so when I check for 0 it doesnt return true
  prevPopulation = 0;
  roundNum = 0; //start at Generation 0
}
//add up the sum of all the surrounding squares (1 if occupied, 0 if not) and return
int GameBoard::countNeighbors(int r, int c)
{
  int sum = 0;
  sum+= neighborValue(r-1,c); //up 1
  sum+= neighborValue(r-1,c+1);//up 1 right 1
  sum+= neighborValue(r,c+1);//right 1
  sum+= neighborValue(r+1,c+1);//down 1 right 1
  sum+= neighborValue(r+1,c);//down 1
  sum+= neighborValue(r+1,c-1);//down 1 left 1
  sum+= neighborValue(r,c-1);//left 1
  sum+= neighborValue(r-1,c-1);//up 1 left 1

  return sum;

}


int GameBoard::neighborValue(int r, int c)
{
  //if neighbor is an edge
  if(r<0 || c<0 || c==columns || r==rows)
  {
    if(mode == 1)
    {
      return 0; //for classic mode return 0
    }
    else if(mode == 2) //mirror mode set it equal to mirrored image
    {
      if(r<0)
      {
        r=0;
      }
      else if(r == rows)
      {
        r = rows-1;
      }
      if(c<0)
      {
        c=0;
      }
      else if(c == columns)
      {
        c = rows -1;
      }
    }
    else //doughnut mode set it equal to opposite space
    {
      if(r<0)
      {
        r = rows-1;
      }
      else if(r == rows)
      {
        r = 0;
      }
      if(c<0)
      {
        c = columns -1;
      }
      else if(c == columns)
      {
        c = 0;
      }
    }

  }

  return currentBoard[r*columns+c];

}

//set currentBoard space to given value
void GameBoard::populateSpace(int value, int r, int c)
{
  currentBoard[r*columns+c] = value;
}
//set the next board spart to given value
void GameBoard::assignSpace(int value, int r, int c)
{
  nextBoard[r*columns+c] = value;
}
//set game mode
void GameBoard::setMode(int givenMode)
{
  mode = givenMode;
}
//set output mode
void GameBoard::setOutputMode(int givenMode)
{
  outputMode = givenMode;
}
//return value of that space on current board
int GameBoard::thisSpace(int r, int c)
{
  return currentBoard[r*columns+c];
}
//set current board to next board and clear next board
void GameBoard::nextGeneration()
{
  currentBoard = nextBoard;
  nextBoard = new int[rows*columns];
  prevPopulation = currentPopulation;//set prev population to current population
  currentPopulation = -1;//clear current population
  roundNum++;//increment round number
}

void GameBoard::outputBoard()
{
  stringstream ss;
  ss << roundNum;
  string strRoundNum = ss.str();
  output(strRoundNum); //output round number
  int r,c;
  currentPopulation++;
  string buildRow = "";
  for(r=0; r<rows; ++r)//run through rows and columns and build row depending on value
  {
    for(c=0;c<columns; ++c)
    {
      if(currentBoard[r*columns+c]==0)
      {
        buildRow +="-";
      }
      else
      {
        currentPopulation++;
        buildRow +="X";
      }
    }
    output(buildRow);//output row
    buildRow = "";


  }

}
//output given string
//if output mode is to file (3) write string to file
//else print to console
void GameBoard::output(string printThis)
{
  ofstream fout;
  if(outputMode==3)
  {
    fout.open(outputFile, std::fstream::app);
    if (fout.fail( ))
    {
      cout << "Output file opening failed.\n";
      exit(1);
    }
    fout<<printThis<<endl;
    fout.close();
  }
  else
  {
    cout<<printThis<<endl;
  }
}
