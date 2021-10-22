// Copyright 2019 Jakob Sailer
// Author: Jakob Sailer <sailer.jakob@web.de>

#include "./board.h"

#ifndef CONDITIONS_H_
#define CONDITIONS_H_

// adds a coin to the board
// returns 0 if not possible
int add_coin(struct board *b, int x, int ply);

// returns 1 if game has ended. Calls below functions
int check_game(struct board *b);

// counts all connecting coins of same color and return amount
int check_vertical(struct board *b, int x, int y, int ply);
int check_horizontal(struct board *b, int x, int y, int ply);
int check_decline(struct board *b, int x, int y, int ply);
int check_incline(struct board *b, int x, int y, int ply);

#endif  // CONDITIONS_H_
