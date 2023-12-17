#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <ctime>
#include "windows.h"
#include <vector>

using namespace std;

int foodX, foodY, startTime;
int inGameTime = 0;
int score = 0;
int startX = 10;
int startY = 10;

//snake parts
vector<int> snakeX{1};
vector<int> snakeY{1};
int snakeLength = 1;

bool gameOver;

enum direction {
  LEFT = 0,
  UP,
  RIGHT,
  DOWN
};

direction currentDirection;

vector<string> mapWall{
 string("NNNNNNNNNNNNNNNNNNNNN"), 
 string("N                   N"),
 string("N                   N"),
 string("N                   N"),
 string("N                   NNNNNNNNNNNNNNNNNNNNNNN"),
 string("N                                         N"),
 string("N                   NNNNNNNNNNNNNNNNNNNNN N"),
 string("N                   N score:            N N"),
 string("N                   N                   N N"),
 string("N                   N  time:            N N"),
 string("N                   N                   N N"),
 string("N                   NNNNNNNNNNNNNNNNNNNNN NNNNNNNNNNNNNNNN"),
 string("N                                                        N"),
 string("N                                                        N"),
 string("N                                                        N"),
 string("N                                                        N"),
 string("N                                                        N"),
 string("NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN")
};

vector<string> map = mapWall;

bool contains(vector<int> array, int value) {
  for (int coordinate : array) {
    if (coordinate == value) {
      return true;
    }
  }
  return false;
}

void newFood() {
  do{
    foodX = rand() % map.back().size();
    foodY = rand() % map.size();
  } while ( ( contains(snakeX, foodX) && contains(snakeY, foodY) ) || (foodY >= 6 && foodY <= 11 && foodX >= 20 && foodX <= 40) || foodX > map[foodY].size() - 1 || mapWall[foodY][foodX] == 'N');
}

// start values
void setup() {
  gameOver = false;

  currentDirection = LEFT;
  
  snakeY[0] = startY;
  snakeX[0] = startX;
  
  startTime = time(NULL);

  srand( time(NULL) );
  
  newFood();
  
  score = 0;
}

//controls
void input() {
  if ( _kbhit() ) {
    switch ( _getch() ) {
      case 'A':
      case 'a':
        if (currentDirection != RIGHT) {
          currentDirection = LEFT;
        }
        break;
      case 'W':
      case 'w':
        if (currentDirection != DOWN) {
          currentDirection = UP;
        }
        break;
      case 'D':
      case 'd':
        if (currentDirection != LEFT) {
          currentDirection = RIGHT;
        }
        break;
      case 'S':
      case 's':
        if (currentDirection != UP) {
          currentDirection = DOWN;
        }
        break;
      case 27:
        gameOver = true;
        break;
    }
  }
}

void moveTail() {
  for (int part = snakeLength - 1; part > 0; --part) {
    snakeX[part] = snakeX[part - 1];
    snakeY[part] = snakeY[part - 1];
  }
}

void logic() {
  char nextBlock;

  switch (currentDirection) {
    case UP:
      nextBlock = map[snakeY[0] - 1][snakeX[0]];

      if (nextBlock == '@') {
        snakeX.push_back(snakeX[snakeLength - 1]);
        snakeY.push_back(snakeY[snakeLength - 1]);

        ++snakeLength;
        ++score;

        newFood();
      } else if ( (nextBlock == 'N' || nextBlock == '#') && !(snakeY[0] - 1 == snakeY[snakeLength - 1] && snakeX[0] == snakeX[snakeLength - 1]) ) {
        gameOver = true;

        break;
      }
      moveTail();

      --snakeY[0];

      break;

    case DOWN:
      nextBlock = map[snakeY[0] + 1][snakeX[0]];

      if (nextBlock == '@') {
        snakeX.push_back(snakeX[snakeLength - 1]);
        snakeY.push_back(snakeY[snakeLength - 1]);

        ++snakeLength;
        ++score;

        newFood();
      } else if ( (nextBlock == 'N' || nextBlock == '#') && !(snakeY[0] + 1 == snakeY[snakeLength - 1] && snakeX[0] == snakeX[snakeLength - 1]) ) {
        gameOver = true;

        break;
      }
      moveTail();

      ++snakeY[0];

      break;

    case RIGHT:
      nextBlock = map[snakeY[0]][snakeX[0] + 1];

      if (nextBlock == '@') {
        snakeX.push_back(snakeX[snakeLength - 1]);
        snakeY.push_back(snakeY[snakeLength - 1]);

        ++snakeLength;
        ++score;

        newFood();
      } else if ( (nextBlock == 'N' || nextBlock == '#') && !(snakeY[0] == snakeY[snakeLength - 1] && snakeX[0] + 1 == snakeX[snakeLength - 1]) ) {
        gameOver = true;

        break;
      }
      moveTail();

      ++snakeX[0];

      break;

    case LEFT:
      nextBlock = map[snakeY[0]][snakeX[0] - 1];

      if (nextBlock == '@') {
        snakeX.push_back(snakeX[snakeLength - 1]);
        snakeY.push_back(snakeY[snakeLength - 1]);

        ++snakeLength;
        ++score;

        newFood();
      } else if ( (nextBlock == 'N' || nextBlock == '#') && !(snakeY[0] == snakeY[snakeLength - 1] && snakeX[0] - 1 == snakeX[snakeLength - 1]) ) {
        
        gameOver = true;

        break;
      }
      moveTail();

      --snakeX[0];

      break;
      }
}

void drawMap () {
  map = mapWall;

  //score and time in square
  string scoreRow = to_string(score);
  string timeRow = to_string(inGameTime / 60) + ":" + to_string( (inGameTime % 60) / 1);
  for (int symbolIndex = 0; symbolIndex < scoreRow.size(); ++symbolIndex) {
    map[7][29 + symbolIndex] = scoreRow[symbolIndex];
  }
  for (int symbolIndex = 0; symbolIndex < timeRow.size(); ++symbolIndex) {
    map[9][29 + symbolIndex] = timeRow[symbolIndex];
  }

  //snake
  for (int part = 0; part < snakeLength; ++part) {
    map[snakeY[part]][snakeX[part]] = '#';
  }
  
  //food
  map[foodY][foodX] = '@';
  
  system("cls");
  
  for (string row : map) {
    cout << row << endl;
  }
}

int main(){
  setlocale(LC_ALL, "Russian");

  int tick = 125;
  
  setup();
  while (gameOver != true) {
    input();

    logic();

    drawMap();

    Sleep(tick);

    inGameTime = time(NULL) - startTime;

    if (inGameTime > 900) {
      gameOver = true;

      cout << endl << "you've been playing for too long. have a rest.";
    }
  }
  
  cout << "you dead" << endl;

  system("pause");
  
  return 0;
}
