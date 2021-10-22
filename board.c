// Copyright 2019 Jakob Sailer
// Author: Jakob Sailer <sailer.jakob@web.de>

#include <stdio.h>

#include "./board.h"
#include "./saveload.h"

int init_board(struct board *b,  FILE *f) {
  // flush array
  for (int i = 0; i < NUM_Y * SIZE_Y + 2; i++) {
    for (int j = 0; j < NUM_X * SIZE_X + 2; j++) {
      b->visual[j][i] = '0';
    }
  }
  for (int i = 0; i < NUM_Y; i++) {
    print_left(b, i);
    for (int j = 0; j < NUM_X; j++) {
      // flush all arrays and print cells to char array
      b->coins[j][i] = 0;
      b->lastmove[0] = 0;
      b->lastmove[1] = 0;
      print_cell(b, f, j, i);
    }
    // after a row of cells is printed add a newline char
    print_newline(b, i);
  }
  // after all cells are printed add the bottom
  print_bottom(b);
  return 1;
}

int print_newline(struct board *b, int y) {
  y *= SIZE_Y;
  for (int i = 0; i < SIZE_Y; i++) {
    b->visual[NUM_X * SIZE_X + 1][y + i] = 'N';
  }
  return 1;
}

int print_left(struct board *b, int y) {
  y *= SIZE_Y;
  for (int i = 0; i < SIZE_Y; i++) {
    b->visual[0][y + i] = '|';
  }
  return 1;
}

int print_bottom(struct board *b) {
  for (int i = 0; i < NUM_X * SIZE_X + 1; i++) {
    // divide bottom into cellwidth to either print '|' or '-'
    // I have to type SIZE_X in backets for some reason
    // or it wont work ?!
    if (i % (SIZE_X) == 0) {
      b->visual[i][NUM_Y * SIZE_Y] = '|';
    } else {
      b->visual[i][NUM_Y * SIZE_Y] = '-';
    }
    // add a newline char at the end
    b->visual[NUM_X * SIZE_X + 1][NUM_Y * SIZE_Y] = 'N';
    // for the next row add numbers to each column
    // they should be in the middle of the column
    if ((i - SIZE_X/2) % (SIZE_X) == 0) {
      b->visual[i][NUM_Y * SIZE_Y + 1] = 49 + (i / (SIZE_X));
    } else {
      b->visual[i][NUM_Y * SIZE_Y + 1] = ' ';
    }
    // add another newline
    b->visual[NUM_X * SIZE_X + 1][NUM_Y * SIZE_Y + 1] = 'N';
  }
  return 1;
}

int print_cell(struct board *b, FILE *f, int x, int y) {
  // each cell has SIZE_Y*SIZE_X "pixels"
  x *= SIZE_X;
  y *= SIZE_Y;
  y++;  // because of '|' at left side

  // top of the cell
  for (int i = 1; i < SIZE_X; i++) {
    b->visual[x + i][y - 1] = '-';
  }

  // close the cell with '|' at the right
  for (int i = 0; i < SIZE_Y; i++) {
    b->visual[x + SIZE_X][y + i - 1] = '|';
  }

  // fill the cells with corresponding chars
  if (b->read) {
    int ply = load(f);
    if (ply) {
      print_ply(b, x, y, ply);
      return 1;
    }
  }
  // if not reading from save or save has '0'
  print_empty(b, x, y);
  return 1;
}

int print_empty(struct board *b, int x, int y) {
  for (int i = 0; i < SIZE_Y - 1; i++) {
    for (int j = 1; j < SIZE_X; j++) {
      b->visual[x + j][y + i] = ' ';
    }
  }
  return 1;
}

int print_ply(struct board *b, int x, int y, int ply) {
  // we need the cell number again instead of cell "pixels"
  x /= SIZE_X;
  y /= SIZE_Y;

  // add the players coin to the coin-matrix
  b->coins[x][y] = ply;

  // fill a whole cell with a coin char
  for (int i = 1; i < SIZE_Y; i++) {
    for (int j = 1; j < SIZE_X; j++) {
      b->visual[x * SIZE_X + j][y * SIZE_Y + i] = 'O';
    }
  }
  return 1;
}
