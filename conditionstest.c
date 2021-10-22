// Copyright 2019 Jakob Sailer
// Author: Jakob Sailer <sailer.jakob@web.de>

#include <assert.h>

#include "./board.h"
#include "./saveload.h"
#include "./conditions.h"

int main(void) {
  struct board b;
  char *filename = "save2.4";
  FILE *f = fopen(filename, "r");
  init_board(&b, f);
  fclose(f);

  // ply1 makes a move
  assert(add_coin(&b, 3, 1));
  assert(b.coins[3][5] == 1);

  assert(check_vertical(&b, 3, 5, 1) == 1);
  assert(check_horizontal(&b, 3, 5, 1) == 1);
  assert(check_incline(&b, 3, 5, 1) == 1);
  assert(check_decline(&b, 3, 5, 1) == 1);

  assert(check_vertical(&b, 3, 5, 2) == 0);
  assert(check_horizontal(&b, 3, 5, 2) == 0);
  assert(check_incline(&b, 3, 5, 2) == 0);
  assert(check_decline(&b, 3, 5, 2) == 0);

  assert(check_game(&b) == 0);

  // ply2 makes a move
  assert(add_coin(&b, 2, 2));
  assert(b.coins[2][5] == 2);

  assert(check_vertical(&b, 2, 5, 1) == 0);
  assert(check_horizontal(&b, 2, 5, 1) == 0);
  assert(check_incline(&b, 2, 5, 1) == 0);
  assert(check_decline(&b, 2, 5, 1) == 0);

  assert(check_vertical(&b, 2, 5, 2) == 1);
  assert(check_horizontal(&b, 2, 5, 2) == 1);
  assert(check_incline(&b, 2, 5, 2) == 1);
  assert(check_decline(&b, 2, 5, 2) == 1);

  assert(check_game(&b) == 0);

  // ply1 makes a move
  assert(add_coin(&b, 3, 1));
  assert(b.coins[3][4] == 1);

  assert(check_vertical(&b, 3, 4, 1) == 2);
  assert(check_horizontal(&b, 3, 4, 1) == 1);
  assert(check_incline(&b, 3, 4, 1) == 1);
  assert(check_decline(&b, 3, 4, 1) == 1);

  assert(check_vertical(&b, 3, 4, 2) == 0);
  assert(check_horizontal(&b, 3, 4, 2) == 0);
  assert(check_incline(&b, 3, 4, 2) == 0);
  assert(check_decline(&b, 3, 4, 2) == 0);

  assert(check_game(&b) == 0);

  // ply2 makes a move
  assert(add_coin(&b, 2, 2));
  assert(b.coins[2][4] == 2);

  assert(check_vertical(&b, 2, 4, 1) == 0);
  assert(check_horizontal(&b, 2, 4, 1) == 0);
  assert(check_incline(&b, 2, 4, 1) == 0);
  assert(check_decline(&b, 2, 4, 1) == 0);

  assert(check_vertical(&b, 2, 4, 2) == 2);
  assert(check_horizontal(&b, 2, 4, 2) == 1);
  assert(check_incline(&b, 2, 4, 2) == 1);
  assert(check_decline(&b, 2, 4, 2) == 1);

  assert(check_game(&b) == 0);

  assert(add_coin(&b, 3, 1));
  assert(add_coin(&b, 2, 2));
  assert(add_coin(&b, 2, 1));
  assert(add_coin(&b, 2, 2));
  assert(add_coin(&b, 2, 1));
  assert(!add_coin(&b, 2, 2));
  assert(add_coin(&b, 1, 2));

  assert(add_coin(&b, 3, 1));
  assert(check_vertical(&b, 3, 2, 1) == 4);
  assert(check_game(&b) == 1);
  return 0;
}
