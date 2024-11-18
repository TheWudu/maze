#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>

#define C_WALL '#'
#define C_WALL_CURSOR 'X'
#define C_CURSOR '+'
#define C_START 'S'
#define C_END 'E'
#define C_PATH 'o'
#define C_WALK '*'

#define SIZE 40
#define C_SLEEP 50

void init_walls(char maze[SIZE][SIZE], size_t size) 
{
  for(size_t i = 0; i < size; i++) {
    for(size_t j = 0; j < size; j++) {
      maze[i][j] = ' ';
    }
  }
  
  for(size_t i = 0; i < size; i++) 
  {
    maze[0][i] = C_WALL;
    maze[size-1][i] = C_WALL;
    maze[i][0] = C_WALL;
    maze[i][size-1] = C_WALL;
  }

  maze[1][0] = C_START;
  maze[SIZE-2][SIZE-1] = C_END;
}

void print_maze(char maze[SIZE][SIZE], size_t size) 
{
  system("clear");
  system("stty cooked"); 
  char val = 0;

  for(size_t i = 0; i < size; i++) {
    for(size_t j = 0; j < size; j++) {
      val = maze[i][j];
      if(val == C_PATH)
      { std::cout << "\033[1;32m"; }
      else if(val == C_WALK)
      { std::cout << "\033[1;31m"; }
      else if(val == C_START)
      { std::cout << "\033[1;31m"; }
      else if(val == C_END)
      { std::cout << "\033[1;32m"; }
      else if(val == C_CURSOR || val == C_WALL_CURSOR)
      { std::cout << "\033[1;34m"; }
      else
      { std::cout << "\033[1;37m"; }
      std::cout << maze[i][j];
    }
    std::cout << std::endl;
  }
}


int run(char maze[SIZE][SIZE], size_t size, int posx, int posy, bool dry = false)
{
  int res = -1;

  if(maze[posy][posx] == C_WALL || 
     maze[posy][posx] == C_WALK ||
     maze[posy][posx] == C_START )
  {
    return -1;
  }
  if(maze[posy][posx] == C_END)
  {
    return 0;
  }
  if(dry == false)
  {
    print_maze(maze, SIZE);
    //std::cout << "Pos: " << posy << " / " << posx << std::endl;
    // sleep(0.75);
    std::this_thread::sleep_for(std::chrono::milliseconds(C_SLEEP));
  }
  maze[posy][posx] = C_WALK;

  res = run(maze, size, posx+1, posy, dry);  // move right
  if(res == 0) {
    maze[posy][posx] = C_PATH;
    return 0;
  }
  res = run(maze, size, posx, posy+1, dry);  // move down
  if(res == 0) {
    maze[posy][posx] = C_PATH;
    return 0;
  }
  res = run(maze, size, posx-1, posy, dry);  // move left
  if(res == 0) {
    maze[posy][posx] = C_PATH;
    return 0;
  }
  res = run(maze, size, posx, posy-1, dry);  // move up
  if(res == 0) {
    maze[posy][posx] = C_PATH;
    return 0;
  }

  return -1;
}

int random_fill(char maze[SIZE][SIZE], size_t size) {
  int val = 0;
  char block = ' ';

  do
  {
    for(size_t i = 1; i < size - 1; i++) {
      for(size_t j = 1; j < size - 1; j++) {
        val = rand() % 3;
        block = ' ';
        switch(val) {
          case 1: block = C_WALL; break;
          case 2: block = ' '; break;
          case 3: block = ' '; break;
          default: break;
        }
        maze[j][i] = block; 
      }
    }
  }
  while( run(maze, SIZE, 1, 1, true) != 0 );
    
  for(size_t i = 1; i < size - 1; i++) {
    for(size_t j = 1; j < size - 1; j++) {
      if(maze[j][i] != C_WALL)
        maze[j][i] = ' '; 
    }
  }
      
  print_maze(maze, SIZE);

  return 0;
}

int main()
{
  char maze[SIZE][SIZE] = {0x20};
  int posx = 1;
  int posy = 1;
  char prev_block = ' ';
  int res = 0;

  init_walls(maze, SIZE);

  char input = 0;
  do {
    prev_block = maze[posy][posx];
    if(prev_block == C_WALL)
      maze[posy][posx] = C_WALL_CURSOR;
    else
      maze[posy][posx] = C_CURSOR;


    print_maze(maze, SIZE);
    std::cout << "Cursor: " << posy << " / " << posx << std::endl;
    printf("input: %d\n", input);
  
    system("stty raw"); 
    input = std::getchar(); 
    system("stty cooked"); 

    maze[posy][posx] = prev_block;

    switch(input) {
      case 66: posy++; break;
      case 67: posx++; break;
      case 65: posy--; break;
      case 68: posx--; break;
      case 'c': 
        init_walls(maze, SIZE);
        break;
      case 'f':
        random_fill(maze, SIZE);
        break;
      case ' ': 
        if(maze[posy][posx] == C_WALL)
        {  maze[posy][posx] = ' '; }
        else
        {  maze[posy][posx] = C_WALL; }
        break;
      case 'r':
        res = run(maze, SIZE, 1, 1, false);
        print_maze(maze, SIZE);
        if (res == 0)
        {
          std::cout << "\033[1;32m";
          std::cout << "!!! Found it !!!" << std::endl;
          std::cout << "\033[1;37m";
        }
        else
        {
          std::cout << "\033[1;31m";
          std::cout << "No way out .. " << std::endl;
          std::cout << "\033[1;37m";
        }
        sleep(5);
        break;
      default: break;
    }

    prev_block = maze[posy][posx];

    if(posx < 1)
      posx = 1;
    if(posx >= SIZE)
      posx = SIZE-1;
    if(posy < 1)
      posy = 1;
    if(posy >= SIZE)
      posy = SIZE-1;
  }
  while(input != 'q');

  return 0;
}

