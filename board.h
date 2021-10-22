// Copyright 2019 Jakob Sailer
// Author: Jakob Sailer <sailer.jakob@web.de>

#include <stdio.h>
#include <stdbool.h>

#ifndef BOARD_H_
#define BOARD_H_

#define WIN 4

#define NUM_X 7
#define NUM_Y 6

#define SIZE_Y 5
#define SIZE_X SIZE_Y*2

struct board {
  // 0 for no coin, 1 and 2 for players
  int coins[NUM_X][NUM_Y];
  // hight and width for each cell:
  // NUM_Y: +1 for newline chars, +1 for '|' column on left side
  // NUM_X: +1 for lowest row, +1 for numberation
  char visual[NUM_X * SIZE_X + 2][NUM_Y * SIZE_Y + 2];
  // remember whether or not to read from a savefile
  bool read;
  // saves last playermove
  int lastmove[2];  // x, y
  // ai or two player game mode
  int ai;
};

// initializes a board. It might be empty or
// contain coins from a savefile
int init_board(struct board *b, FILE *f);

// each row ends with a char to indicate a newline for the frameprinter
int print_newline(struct board *b, int y);

// since a cell only has a top and right we need these two:
int print_left(struct board *b, int y);
int print_bottom(struct board *b);

// prints rest of cell. Calls functions below it
int print_cell(struct board *b, FILE *f, int x, int y);

// these two take care of what to fill a cell with
int print_empty(struct board *b, int x, int y);
int print_ply(struct board *b, int x, int y, int ply);

#endif  // BOARD_H_
