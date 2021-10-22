// Copyright 2019 Jakob Sailer
// Author: Jakob Sailer <sailer.jakob@web.de>

#include <stdio.h>

#include "./board.h"

#ifndef SAVELOAD_H_
#define SAVELOAD_H_

// checks if existing savefile is valid
// returns player whos turn it is if valid
int parse_save(char *s);

// creates a savefile
int save(struct board *b);

// returns player color from next savechar
int load(FILE *f);

#endif  // SAVELOAD_H_
