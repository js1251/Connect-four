// Copyright 2019 Jakob Sailer
// Author: Jakob Sailer <sailer.jakob@web.de>

#include "./board.h"

#ifndef FRAME_H_
#define FRAME_H_

// prints a frame of the current board
int print_frame(struct board *b);

// prints an invalid input message when function arguments were wrong
int print_error(int i);

// prints the readme.txt to the console
int print_help();

#endif  // FRAME_H_
