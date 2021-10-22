// Copyright 2019 Jakob Sailer
// Author: Jakob Sailer <sailer.jakob@web.de>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./board.h"
#include "./frame.h"
#include "./saveload.h"

int parse_save(char *s) {
  FILE *f = fopen(s, "r");
  // check if file can even be opened
  if (!f) {
    print_error(0);
    printf("\"%s\"\n", s);
    return 0;
  }

  bool tempboard[NUM_X][NUM_Y];
  for (int i = 0; i < NUM_X; i++) {
    for (int j = 0; j < NUM_Y; j++) {
      tempboard[i][j] = 0;
    }
  }
  int x = 0, y = 0;
  int ply1 = 0, ply2 = 0;
  char check;
  for (int i = 0; i < NUM_X * NUM_Y; i++) {
    // check if all chars are the numbers '0', '1' or '2'
    if (!feof(f)) {
      check = fgetc(f) - '0';
      if (check >= 0 && 3 > check) {
        if (check == 1 && i != NUM_X * NUM_Y) {
          tempboard[x][y] = 1;
          ply1++;
        } else if (check == 2 && i != NUM_X * NUM_Y) {
          tempboard[x][y] = 1;
          ply2++;
        }
        x++;
        if (x == NUM_X) {
          y++;
          x = 0;
        }
      } else {
        // a different char was found
        print_error(1);
        return 0;
      }
    } else {
      // if file has less chars than spots on the board the save is too short
      print_error(2);
      return 0;
    }
  }
  // check if theres no floating coins
  for (int i = 0; i < NUM_Y; i++) {
    for (int j = 0; j < NUM_X; j++) {
      if (tempboard[j][i] && !(i == NUM_Y-1 || tempboard[j][i + 1])) {
        print_error(5);
        return 0;
      }
    }
  }
  if (fgetc(f) != EOF) {
    // if theres still a char after the for loop the file is two long
    print_error(3);
    return 0;
  }
  if (ply1 - ply2 > 1 || ply2 - ply1 > 1) {
    // if the difference between playercoins is more than one the balance is off
    print_error(4);
    return 0;
  }
  // return whos turn it is if the savefile checks out
  // we can return this even if ply1 == ply2 as player 1 always starts
  return (ply1 > ply2) + 1;
}

int save(struct board *b) {
  // ask user for a name
  printf("Specify a savefilename: ");
  char input[255] = "";
  scanf("%s", input);
  // concat ".4" to filename
  strcat(input, ".4");
  FILE *f = fopen(input, "w");
  for (int i = 0; i < NUM_Y; i++) {
    for (int j = 0; j < NUM_X; j++) {
      // print each coin to savefile
      fprintf(f, "%d", b->coins[j][i]);
    }
  }
  fclose(f);
  return 1;
}

int load(FILE *f) {
  return fgetc(f) - '0';
}
