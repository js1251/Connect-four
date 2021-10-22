// Copyright 2019 Jakob Sailer
// Author: Jakob Sailer <sailer.jakob@web.de>

#include "./board.h"

#ifndef AI_H_
#define AI_H_

struct ai {
  // an array pointing to the first coin in each column
  int check[NUM_X];
  // how many coins away for opponent win
  int risk[NUM_X];
  // how many coins away for ai win
  int chance[NUM_X];
  // array of options to place coin at
  int options[NUM_X];
};

int ai_reset(struct ai *a);

// adds values that tell the ai where best to place a coin
// calls functions below
int ai_weighboard(struct board *b, struct ai *a);

// calculates the value for all board directions
int ai_vertical(struct board *b, struct ai *a, int x, int y);
int ai_horizontal(struct board *b, struct ai *a, int x, int y, int ply);
int ai_decline(struct board *b, struct ai *a, int x, int y, int ply);
int ai_incline(struct board *b, struct ai *a, int x, int y, int ply);

// decides where to place a new coin
int ai_makemove(struct board *b, struct ai *a);

// adds the weight values to the boards charmatrix (ai debug mode only)
int ai_addvalues(struct board *b, struct ai *a);

// removes the weight values from the boards charmatrix (ai debug mode only)
int ai_removevalues(struct board *b, struct ai *a);

#endif  // AI_H_
