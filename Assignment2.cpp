#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include "gameBoard.h"
#include <unistd.h>


using namespace std;

void playGame(GameBoard &thisBoard);
void setup(GameBoard &thisBoard);
void fileInput(GameBoard &thisBoard);
void randomInput(GameBoard &thisBoard);
void fileOutput(GameBoard &thisBoard);
void letsPlay();
unsigned int sleep(unsigned int seconds);


int main(int argc, char** argv)
{
  letsPlay();//play game
  cout<<"Press enter to exit the program"<<endl;
  cin.get();

}

void letsPlay()
{
  GameBoard thisBoard;//create GameBoard
  int stableRounds = 0;
  setup(thisBoard);//setup board

  thisBoard.outputBoard();//output original board
  //handle output mode (pause or wait for enter if output mode is 1 or 2)
  if(thisBoard.outputMode == 1)
  {
    sleep(2);
  }
  else if(thisBoard.outputMode == 2)
  {
    cin.get();
  }
  //continue to play game and handle output mode until population hits 0 or stabilizes
  while(thisBoard.currentPopulation!=0 && stableRounds<6)
  {
    if(thisBoard.outputMode == 1)
    {
      sleep(2);
    }
    else if(thisBoard.outputMode == 2)
    {
      cin.get();
    }
    playGame(thisBoard);
    if((thisBoard.prevPopulation-thisBoard.currentPopulation) == 0)
    {
      stableRounds++;
    }
    else
    {
      stableRounds = 0;
    }


  }
}


void setup(GameBoard &thisBoard)
{
  char userInput = '0';

  while(userInput != '1' && userInput != '2')
  {
    //prompte for input type and navigate accordingly
    cout<<"How would you like to input the game board?"<<endl;
    cout<<"1) Input Map File"<<endl;
    cout<<"2) Random Assignment"<<endl;
    cin>>userInput;

    if(userInput == '1')
    {
      fileInput(thisBoard);
    }
    else if(userInput == '2')
    {
      randomInput(thisBoard);
    }
    else
    {
      cout<<"Incorrect input, enter a 1 or 2"<<endl;
    }
  }
  userInput = '0';
  int charToInt = 0;
  while(userInput != '1' && userInput != '2' && userInput != '3')
  {
    //prompt for game mode and set game mode accordingly
    cout<<"What mode would you like to play in?"<<endl;
    cout<<"1)Classic Mode"<<endl;
    cout<<"2)Mirror Mode"<<endl;
    cout<<"3)Doughnut Mode"<<endl;
    cin>>userInput;
    if(userInput == '1' || userInput == '2' || userInput == '3')
    {
      charToInt = userInput - '0';
      thisBoard.setMode(charToInt);
    }
  }
  userInput = 0;
  while(userInput != '1' && userInput != '2' && userInput != '3')
  {
    //prompt for output type and set output mode accordinging
    cout<<"How would you like to output results?"<<endl;
    cout<<"1)Pause in between each generation"<<endl;
    cout<<"2)Wait for me to press Enter in between each generation"<<endl;
    cout<<"3)Output to file"<<endl;
    cin>>userInput;
    if(userInput == '1' || userInput == '2' || userInput == '3')
    {
      charToInt = userInput - '0';
      thisBoard.setOutputMode(charToInt);
      if(userInput == '3')
      {
        fileOutput(thisBoard);
      }
    }
  }

}

void fileInput(GameBoard &thisBoard)
{
  //prompt for a file name
  string fileName = "";
  cout<<"Enter name of file"<<endl;
  cin>>fileName;
  int rowNum = 0;
  int columnNum = 0;
  ifstream givenFile(fileName);
  int currentRow = 0;

  for(string line; getline( givenFile, line ); )
  {
    //read in the row numbers and column numbers
    if(rowNum == 0 && columnNum == 0)
    {
      rowNum = atoi(line.c_str());
      getline(givenFile,line);
      columnNum = atoi(line.c_str());
      thisBoard.createBoard(rowNum,columnNum);
      continue;
    }
    //after wards runthrough the rest of the file line by line
    for(std::string::size_type i = 0; i < line.size(); ++i)
    {
      if(line[i]=='-')//if the char is - then that space has a value of 0
      {
        thisBoard.populateSpace(0,currentRow,i);
      }
      else if(line[i]=='X')//if the char is X then that space has a value of 1
      {
        thisBoard.populateSpace(1,currentRow,i);
      }
    }
    currentRow++;
  }
}

//if random input is selected
void randomInput(GameBoard &thisBoard)
{
  int r,c;
  int rowNum = 0;
  int columnNum = 0;
  double popDensity = 0.0;
  int randNum = 0;
  //prompt for rows and numbers
  cout<<"Enter number of rows: "<<endl;
  cin>>rowNum;
  cout<<"Enter number of columns: "<<endl;
  cin>>columnNum;

  thisBoard.createBoard(rowNum,columnNum);

  //prompt for population density
  while(popDensity<=0 || popDensity>1)
  {
    cout<<"Enter a nonzero decimal value between 0 and 1 for population density: "<<endl;
    cin>>popDensity;
  }
  popDensity = popDensity*100;
  cout<<"Population density: "<<popDensity<<'%'<<endl;

  //for every space in the board
  for(r=0; r<rowNum; ++r)
  {
    for(c=0;c<columnNum; ++c)
    {
      randNum = rand()%100; //generate random number between 0 and 100
      if(randNum<=popDensity)//if the number is less than or equal to the density
      {
        thisBoard.populateSpace(1,r,c);//that space is populated (==1)
      }
      else
      {
        thisBoard.populateSpace(0,r,c);//that space is not populated (==0)
      }
    }
  }

}
void fileOutput(GameBoard &thisBoard)
{
  string fileName = "";
  cout<<"Enter name of file"<<endl;//prompt for output file name
  cin>>fileName;
  thisBoard.outputFile = fileName;//set output file name to this file
}

void playGame(GameBoard &thisBoard)
{
  int r,c;
  int neighbors = 0;
  //run through every space in board
  for(r=0; r<thisBoard.rows; ++r)
  {
    for(c=0;c<thisBoard.columns; ++c)
    {
      neighbors = thisBoard.countNeighbors(r,c);//count the neighbors
      if(neighbors<=1)//if neighbors is less than 1
      {
        thisBoard.assignSpace(0,r,c);//the space in the next generation is unpopulated (==0)
      }
      else if(neighbors==2)//if neighbors is 2
      {
        //the space in the next generation is the same as the current space
        thisBoard.assignSpace(thisBoard.thisSpace(r,c),r,c);
      }
      else if(neighbors == 3)//if neighbors is 3
      {
        //the space in the next generation is populated
        thisBoard.assignSpace(1,r,c);
      }
      else
      {//otherwise the space in the next generation is unpopulated
        thisBoard.assignSpace(0,r,c);
      }

    }
  }
  thisBoard.nextGeneration();//call the next generation
  thisBoard.outputBoard();//output the board
}
