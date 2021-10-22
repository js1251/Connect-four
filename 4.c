// Copyright 2019 Jakob Sailer
// Author: Jakob Sailer <sailer.jakob@web.de>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "./board.h"
#include "./frame.h"
#include "./conditions.h"
#include "./saveload.h"
#include "./ai.h"

int main(int argc, char *argv[]) {
  // there can only be 6 arguments total (+1 for program)
  if (argc > 7) {
    printf("Too many arguments! (max 6)\n");
    return -1;
  }

  // create all neccessary things
  struct board b;
  struct ai a;
  char *filename = NULL;
  int size = 3;
  b.read = false;
  b.ai = 0;

  // read all arguments
  for (int i = 1; i < argc; i++) {
    // check for argument "--help"
    if (strcmp(argv[i], "--help") == 0) {
      print_help();
      return 1;

    // check for argument "--load <file>"
    } else if (strcmp(argv[i], "--load") == 0) {
      if (argv[++i][0] != '-') {
        filename = argv[i];
        b.read = true;
      } else {
        // print invalid input error
        print_error(6);
        return 0;
      }

    // check for argument "--ai <?debug>"
    } else if (strcmp(argv[i], "--ai") == 0) {
      b.ai = 1;
      i++;
      if (argv[i] && strcmp(argv[i], "debug") == 0) {
        b.ai = 2;
      } else if (argv[i] != NULL) {
        // print invalid input error
        print_error(6);
        return 0;
      }

    // check for argument "--size <int>"
    } else if (strcmp(argv[i], "--size") == 0) {
      i++;
      if (argv[i] && isdigit(argv[i][0])) {
        size = atoi(argv[i]);
        if (size < 3) {
          size = 3;
        }
        // dynamically chance charsize here
        printf("THIS FEATURE WAS NOT IMPLEMENTED\n");
        printf("Press Enter to continue. Size will be 3 instead of %d\n", size);
        getc(stdin);
      } else {
        // print invalid input error
        print_error(6);
        return 0;
      }
    } else {
    // if there are arguments but they dont apply to any above
    // they must be invalid so print invalid input error
    print_error(6);
    return 0;
    }
  }

  // check if file savefile checks out if there is one to load
  if (b.read) {
    if (!parse_save(filename)) {
      return 0;
    }
  }

  // initialize game
  FILE *f = fopen(filename, "r");
  init_board(&b, f);
  if (f) {
    fclose(f);
  }
  bool turn = false;
  int checksum = 0;
  print_frame(&b);

  // actual game loop. Keeps going if noone won or its a draw
  while (!check_game(&b) && checksum != NUM_X * NUM_Y) {
    // different interface depending on ai or two player mode
    if (!b.ai) {
      printf("Turn: Player %d: ", turn + 1);
    } else if (turn + 1 == 1) {
      printf("Its your turn: ");
    }
    // for both human players or for only human player if ai is active:
    INPUT: if (!b.ai || turn + 1 == 1) {
      // ask for column to place coin at
      char input[255] = "";
      while (1 > atoi(input) || atoi(input) > NUM_X) {
        scanf("%s", input);
        // see if input was save command
        if (strcmp(input, "S") == 0 || strcmp(input, "s") == 0) {
          save(&b);
          return 1;
        // if it wasnt check if its between 0 and NUM_X
        } else if (1 > atoi(input) || atoi(input) > NUM_X) {
          printf("Input was invalid! ");
          printf("Try a number between 1 and %d: ", NUM_X);
        }
      }
      // try to place the coin. If the column is full return to top
      if (!add_coin(&b, atoi(input)-1, turn + 1)) {
        printf("This Column is already filled! Try again: ");
        goto INPUT;
      }
    } else {
      // do all required calls for the ai to make a move
      ai_reset(&a);
      ai_weighboard(&b, &a);
      // see if debug mode is active
      if (b.ai == 2) {
        // print the risk and chance values to the board, ask the player
        // to continue and remove the values again
        ai_addvalues(&b, &a);
        print_frame(&b);
        ai_removevalues(&b, &a);
        printf("DEBUG: This is what the AI sees! (Enter to continue) ");
        // I guess I have to call getc() twice as some stuff from the previous
        // input is still in some sort of buffer
        getc(stdin);
        getc(stdin);
      }
      // place a coin and frint the new frame
      ai_makemove(&b, &a);
    }
    // add one to checksum and chance whos turn it is,
    // finally print the new frame
    checksum++;
    turn = !turn;
    print_frame(&b);
  }
  // if all spots are taken its a draw
  if (checksum == NUM_X * NUM_Y) {
    printf("The game ended in a draw\n");
  // if the ai is inactive see which player one
  } else if (!b.ai) {
    printf("CONGRATIOLATIONS!! Player %d wins!!\n\n", !turn + 1);
  // if it is active see if the player or the ai won
  } else {
    printf("You %s against the computer!\n\n", turn ? "won" : "lost");
  }
  return 1;
}
