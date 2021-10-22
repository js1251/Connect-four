// Copyright 2019 Jakob Sailer
// Author: Jakob Sailer <sailer.jakob@web.de>

#include <assert.h>

#include "./board.h"
#include "./conditions.h"
#include "./ai.h"

int main(void) {
  struct board b;
  char *filename = "save2.4";
  FILE *f = fopen(filename, "r");
  init_board(&b, f);
  fclose(f);

  struct ai a;
  ai_reset(&a);

  for (int i = 0; i < NUM_X; i++) {
    assert(a.risk[i] == 0);
    assert(a.chance[i] == 0);
  }

  add_coin(&b, 3, 1);
  assert(ai_weighboard(&b, &a));
  assert(a.risk[2] == 1);
  assert(a.risk[3] == 1);
  assert(a.risk[4] == 1);
  assert(a.chance[2] == 0);
  assert(a.chance[3] == 0);
  assert(a.chance[4] == 0);

  add_coin(&b, 2, 2);
  assert(ai_weighboard(&b, &a));
  assert(a.risk[1] == 0);
  assert(a.risk[2] == 1);
  assert(a.risk[3] == 1);
  assert(a.risk[4] == 1);
  assert(a.chance[1] == 1);
  assert(a.chance[2] == 1);
  assert(a.chance[3] == 1);
  assert(a.chance[4] == 0);

  add_coin(&b, 3, 1);
  add_coin(&b, 2, 2);
  add_coin(&b, 3, 1);
  add_coin(&b, 2, 2);
  add_coin(&b, 2, 1);
  assert(ai_weighboard(&b, &a));
  assert(a.risk[2] == -1);
  assert(a.chance[2] == -1);
  assert(a.risk[3] == 3);

  // the ai should prevent a player win now
  ai_makemove(&b, &a);
  assert(b.coins[3][2] == 2);
  return 0;
}
