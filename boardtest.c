// Copyright 2019 Jakob Sailer
// Author: Jakob Sailer <sailer.jakob@web.de>

#include <assert.h>

#include "./board.h"

int main(void) {
  struct board b;
  char *filename = "save1.4";
  FILE *f = fopen(filename, "r");
  init_board(&b, f);
  fclose(f);
  f = fopen(filename, "r");
  // check if all pixels arent empty
  for (int i = 0; i < NUM_Y * SIZE_Y + 2; i++) {
    for (int j = 0; j < NUM_X * SIZE_X + 2; j++) {
      assert(b.visual[j][i]);
    }
  }
  // check if most right column is newline char
  for (int i = 0; i < NUM_Y * SIZE_Y + 2; i++) {
    assert(b.visual[NUM_X * SIZE_X + 1][i] == 'N');
  }
  // check if most left (before newline) is '|'
  for (int i = 0; i < NUM_Y * SIZE_Y + 1; i++) {
    assert(b.visual[0][i] == '|');
  }
  // check if bottom checks out
  for (int i = 0; i < NUM_X * SIZE_X + 1; i++) {
    if (i % (SIZE_X) == 0) {
      assert(b.visual[i][NUM_Y * SIZE_Y] == '|');
    } else {
      assert(b.visual[i][NUM_Y * SIZE_Y] == '-');
    }
    if ((i - SIZE_X/2) % (SIZE_X) == 0) {
      assert(b.visual[i][NUM_Y * SIZE_Y + 1] == 49 + (i / (SIZE_X)));
    } else {
      assert(b.visual[i][NUM_Y * SIZE_Y + 1] == ' ');
    }
  }
  // check cell borders
  for (int i = 0; i < NUM_X * SIZE_X; i += SIZE_X) {
    for (int j = 1; j < NUM_Y * SIZE_Y; j += SIZE_Y) {
      // top of the cell
      for (int k = 1; k < SIZE_X; k++) {
        assert(b.visual[i + k][j - 1] == '-');
      }
      // close the cell with '|' at the right
      for (int k = 0; k < SIZE_Y; k++) {
        assert(b.visual[i + SIZE_X][j + k - 1] == '|');
      }
    }
  }
  // cell content
  for (int i = 0; i < NUM_Y; i++) {
    for (int j = 0; j < NUM_X; j++) {
      char c = fgetc(f);
      for (int k = 1; k < SIZE_Y; k++) {
        for (int l = 1; l < SIZE_X; l++) {
          if (c == '1' || c == '2') {
            assert(b.visual[j * SIZE_X + l][i * SIZE_Y + k] == 'O');
          } else {
            assert(b.visual[j * SIZE_X + l][i * SIZE_Y + k] == ' ');
          }
        }
      }
    }
  }
  fclose(f);
  return 0;
}
