// Copyright 2019 Jakob Sailer
// Author: Jakob Sailer <sailer.jakob@web.de>

#include <stdio.h>
#include <stdlib.h>

#include "./board.h"
#include "./frame.h"

int print_frame(struct board *b) {
  // looks better than to concat the new frame:
  system("clear");
  printf("\"CTRL+C\" to exit, \"S\" to save\n\n");
  // print space till the middle of board
  for (int i = 0; i < (SIZE_X * NUM_X)/2 - 9; i++) {
    printf(" ");
  }
  // print the game name
  printf("====CONNECT FOUR====\n\n");
  // print another row for nicer graphics
  for (int i = 0; i < SIZE_X * NUM_X + 1; i++) {
    if ((i - SIZE_X/2) % (SIZE_X) == 0) {
      printf("V");
    } else {
      printf("_");
    }
  }
  printf("\n");
  // print all cells to console in different colors
  for (int i = 0; i < NUM_Y * SIZE_Y + 2; i++) {
    for (int j = 0; j < NUM_X * SIZE_X + 2; j++) {
      printf("\033[0m");
      if (b->visual[j][i] == 'N') {
        printf("\n");
        continue;
      }
      // the following char check is a hotfix...
      // if you can read this I never fixed it
      if (b->visual[j][i] != '-' && b->visual[j][i] != '|') {
        if (b->coins[j / (SIZE_X)][i / (SIZE_Y)] == 1) {
          // set color to red for ply1
          printf("\033[1;31m");
        } else if (b->coins[j / (SIZE_X)][i / (SIZE_Y)] == 2) {
          // set color to yellow for ply2
          printf("\033[1;33m");
        }
      // the following is needed for showing the ai debug numbers
      }
      if (b->visual[j][i] == '0' && i+1 != NUM_Y * SIZE_Y + 2) {
        printf("\033[1;32m");
      } else if (b->visual[j][i] == '1' && i+1 != NUM_Y * SIZE_Y + 2) {
        printf("\033[0;32m");
      } else if (b->visual[j][i] == '2' && i+1 != NUM_Y * SIZE_Y + 2) {
        printf("\033[0;33m");
      } else if (b->visual[j][i] == '3' && i+1 != NUM_Y * SIZE_Y + 2) {
        printf("\033[1;33m");
      }
      // finally print the current char from the char matrix
      printf("%c", b->visual[j][i]);
    }
  }
  printf("\n\n");
  return 1;
}

int print_error(int i) {
  if (i < 6) {
    printf("Error loading save: ");
  }
  if (i == 0) {
    printf("Couldnt load savefile from ");
  } else if (i == 1) {
    printf("file can only contain '0', '1' or '2'!\n");
  } else if (i == 2) {
    printf("file too short!\n");
  } else if (i == 3) {
    printf("file too long!\n");
  } else if (i == 4) {
    printf("too many coins of one color!\n");
  } else if (i == 5) {
    printf("floating coins!\n");
  } else if (i == 6) {
    printf("Arguments of type --load <file>, ");
    printf("--ai <?debug> or --size <int> expected!\n");
  }
  return 1;
}

int print_help() {
  FILE * f = fopen("readme.txt", "r+b");
  if (!f) {
    printf("ERROR: Couldnt open readme.txt\n");
    return 0;
  }
  char chr = fgetc(f);
  while (chr != EOF) {
    printf("%c", chr);
    chr = fgetc(f);
  }
  printf("\n");
  fclose(f);
  return 1;
}
