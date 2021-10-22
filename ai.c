// Copyright 2019 Jakob Sailer
// Author: Jakob Sailer <sailer.jakob@web.de>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "./board.h"
#include "./conditions.h"
#include "./ai.h"

int ai_reset(struct ai *a) {
  // flush arrays
  for (int i = 0; i < NUM_X; i++) {
    a->risk[i] = 0;
    a->chance[i] = 0;
  }
  return 1;
}

int ai_weighboard(struct board *b, struct ai *a) {
  for (int i = 0; i < NUM_Y; i++) {
    for (int j = 0; j < NUM_X; j++) {
      // for all coins that are not 0 do the following checks
      if (b->coins[j][i] != 0) {
        // for vertical we have to do some additional checking
        // to not overwrite values that shouldnt be overwritten
        if (b->coins[j][i - 1] == 0) {
          ai_vertical(b, a, j, i);
        }
        // do all the rest of the checks its required for both players
        ai_horizontal(b, a, j, i, 1);
        ai_decline(b, a, j, i, 1);
        ai_incline(b, a, j, i, 1);
        ai_horizontal(b, a, j, i, 2);
        ai_decline(b, a, j, i, 2);
        ai_incline(b, a, j, i, 2);
      }
    }
  }
  return 1;
}

int ai_vertical(struct board *b, struct ai *a, int x, int y) {
  int temprisk = check_vertical(b, x, y, 1);
  int tempchance = check_vertical(b, x, y, 2);
  // placing a coin in a column with less than four
  // empties ontop of the opponents coin doesnt make sense
  // (for vertical only might get overwritten)
  if (
    check_vertical(b, x, 2, 1) == 1 ||
    check_vertical(b, x, 2, 2) == 1 ||
    check_vertical(b, x, 1, 1) == 2 ||
    check_vertical(b, x, 1, 2) == 2
  ) {
    a->risk[x] = -1;
    a->chance[x] = -1;
  // if the edgecase doesnt apply calculate how many coins are ontop
  // only overwrite value if its greater than the current one
  } else {
    if (temprisk > a->risk[x]) {
      a->risk[x] = temprisk;
    }
    if (tempchance > a->chance[x]) {
      a->chance[x] = tempchance;
    }
  }
  return 1;
}

int ai_horizontal(struct board *b, struct ai *a, int x, int y, int ply) {
  int temprisk = check_horizontal(b, x, y, 1);
  int tempchance = check_horizontal(b, x, y, 2);

  bool doneright = 0, doneleft = 0;
  int compare;
  // temporary array for either risk or chance
  int weight[NUM_X];

  // depending on player or ai move risk or chance to the temp arrays
  // and temprisk or tempchance to compare
  // this is done to avoid having pretty much the same code twice
  if (ply == 1) {
    compare = temprisk;
    for (int i = 0; i < NUM_X; i++) {
      weight[i] = a->risk[i];
    }
  } else if (ply == 2) {
    compare = tempchance;
    for (int i = 0; i < NUM_X; i++) {
      weight[i] = a->chance[i];
    }
  }

  // go left or right as many spots as compare
  for (int i = 0; i < compare; i++) {
    // check if right is empty
    if (b->coins[x + i + 1][y] == 0 && !doneright) {
      // there needs to be a "support" under the empty
      if (
        (b->coins[x + i + 1][y + 1] || y == NUM_Y - 1) &&
        compare > weight[x + i + 1]
      ) {
        weight[x + i + 1] = compare;
      }
      doneright = 1;
      // else if the next coin is not of the same color right is done
    } else if (b->coins[x + i + 1][y] != ply) {
      doneright = 1;
    }
    // check if left is empty
    if (b->coins[x - i - 1][y] == 0 && !doneleft) {
      // there needs to be a "support" under the empty
      if (
        (b->coins[x - i - 1][y + 1] || y == NUM_Y - 1) &&
        compare > weight[x - i - 1]
      ) {
        weight[x - i - 1] = compare;
      }
      doneleft = 1;
      // else if the next coin is not of the same color left is done
    } else if (b->coins[x - i - 1][y] != ply) {
      doneleft = 1;
    }
  }

  // copy temparray back to either risk or chance
  if (ply == 1) {
    for (int i = 0; i < NUM_X; i++) {
      a->risk[i] = weight[i];
    }
  } else {
    for (int i = 0; i < NUM_X; i++) {
      a->chance[i] = weight[i];
    }
  }
  return 1;
}

int ai_decline(struct board *b, struct ai *a, int x, int y, int ply) {
  int temprisk = check_decline(b, x, y, 1);
  int tempchance = check_decline(b, x, y, 2);

  bool doneright = 0, doneleft = 0;
  int compare;
  // temporary array for either risk or chance
  int weight[NUM_X];

  // depending on player or ai move risk or chance to the temp arrays
  // this is done to avoid having pretty much the same code twice
  if (ply == 1) {
    compare = temprisk;
    for (int i = 0; i < NUM_X; i++) {
      weight[i] = a->risk[i];
    }
  } else if (ply == 2) {
    compare = tempchance;
    for (int i = 0; i < NUM_X; i++) {
      weight[i] = a->chance[i];
    }
  }

  // go left up or right down as many spots as risk
  for (int i = 0; i < compare; i++) {
    // check if right down is empty
    if (!(y + i + 1 > NUM_Y - 1 || x + i + 1 > NUM_X - 1)) {
      if (b->coins[x + i + 1][y + i + 1] == 0 && !doneright) {
        // there needs to be a "support" under the empty
        if (
          (b->coins[x + i + 1][y + i + 2] || y + 1 == NUM_Y - 1) &&
          compare > weight[x + i + 1]
        ) {
          weight[x + i + 1] = compare;
        }
        doneright = 1;
        // else if the next coin is not of the same color right is done
      } else if (b->coins[x + i + 1][y + i + 1] != ply) {
        doneright = 1;
      }
    }
    // check if left up is empty
    if (!(y - i - 1 < 0 || x - i - 1 < 0)) {
      if (b->coins[x - i - 1][y - i - 1] == 0 && !doneleft) {
        // there needs to be a "support" under the empty
        if (
          (b->coins[x - i - 1][y - i]) &&
          compare > weight[x - i - 1]
        ) {
          weight[x - i - 1] = compare;
        }
        doneleft = 1;
        // else if the next coin is not of the same color left is done
      } else if (b->coins[x - i - 1][y - i - 1] != ply) {
        doneleft = 1;
      }
    }
  }

  // copy temparray back to either risk or chance
  if (ply == 1) {
    for (int i = 0; i < NUM_X; i++) {
      a->risk[i] = weight[i];
    }
  } else {
    for (int i = 0; i < NUM_X; i++) {
      a->chance[i] = weight[i];
    }
  }
  return 1;
}

int ai_incline(struct board *b, struct ai *a, int x, int y, int ply) {
  int temprisk = check_incline(b, x, y, 1);
  int tempchance = check_incline(b, x, y, 2);

  bool doneright = 0, doneleft = 0;
  int compare;
  int weight[NUM_X];

  // depending on player or ai move risk or chance to the temp arrays
  // this is done to avoid having pretty much the same code twice
  if (ply == 1) {
    compare = temprisk;
    for (int i = 0; i < NUM_X; i++) {
      weight[i] = a->risk[i];
    }
  } else if (ply == 2) {
    compare = tempchance;
    for (int i = 0; i < NUM_X; i++) {
      weight[i] = a->chance[i];
    }
  }

  // go left down or right up as many spots as compare
  for (int i = 0; i < compare; i++) {
    // check if right up is empty
    if (!(y - i - 1 < 0 || x + i + 1 > NUM_X - 1)) {
      if (b->coins[x + i + 1][y - i - 1] == 0 && !doneright) {
        // there needs to be a coin underneath the empty for it to
        // be a valid option to place a coin at
        if (
          (b->coins[x + i + 1][y - i]) &&
          compare > weight[x + i + 1]
        ) {
          weight[x + i + 1] = compare;
        }
        doneright = 1;
      } else if (b->coins[x + i + 1][y - i - 1] != ply) {
        doneright = 1;
      }
    }
    // check if left down is empty
    if (!(y + i + 1 > NUM_Y - 1 || x - i - 1 < 0)) {
      if (b->coins[x - i - 1][y + i + 1] == 0 && !doneleft) {
        // there needs to be a coin underneath the empty for it to
        // be a valid option to place a coin at
        if (
          (b->coins[x - i - 1][y + i + 2] || y + 1 == NUM_Y - 1) &&
          compare > weight[x - i - 1]
        ) {
          weight[x - i - 1] = compare;
        }
        doneleft = 1;
      } else if (b->coins[x - i - 1][y + i + 1] != ply) {
        doneleft = 1;
      }
    }
  }

  // copy temparray back to either risk or chance
  if (ply == 1) {
    for (int i = 0; i < NUM_X; i++) {
      a->risk[i] = weight[i];
    }
  } else {
    for (int i = 0; i < NUM_X; i++) {
      a->chance[i] = weight[i];
    }
  }
  return 1;
}


int ai_makemove(struct board *b, struct ai *a) {
  // get the highest value for both risk and chance
  int risk = 0, chance = 0;
  for (int i = 0; i < NUM_X; i++) {
    if (a->risk[i] > risk) {
      risk = a->risk[i];
    }
    if (a->chance[i] > chance) {
      chance = a->chance[i];
    }
  }
  // get all columns of those highest values
  int count = 0;
  for (int i = 0; i < NUM_X; i++) {
    // prioritice chance if theres a value of 3
    if (chance == 3) {
      if (a->chance[i] == chance) {
        a->options[count++] = i;
      }
      // else prioritice risk if theres a value of 3
    } else if (risk == 3) {
      if (a->risk[i] == risk) {
        a->options[count++] = i;
      }
      // else if theres a value of two for risk add it and all other
      // chances of 2 to the options
    } else if (risk == 2) {
      if (a->risk[i] == risk) {
        a->options[count++] = i;
      }
      if (a->chance[i] == risk) {
        a->options[count++] = i;
      }
      // else add all highest chances to options
    } else {
      if (a->chance[i] == chance) {
        a->options[count++] = i;
      }
    }
  }
  // choose a random spot from options array
  time_t t;
  srand((unsigned) time(&t));
  int x = a->options[rand() % count];
  // finally add a coin to the choosen spot
  add_coin(b, x, 2);
  return 1;
}

int ai_addvalues(struct board *b, struct ai *a) {
  for (int i = 0 ; i < NUM_X; i++) {
    for (int j = 0; j < NUM_Y; j++) {
      if (b->coins[i][j] != 0 || j == NUM_Y-1) {
        if (j == NUM_Y-1 && b->coins[i][j] == 0) {
          j++;
        }
        b->visual[i * SIZE_X + 2][SIZE_Y * (j-1) + 1] = a->risk[i] + '0';
        b->visual[i * SIZE_X + 2][SIZE_Y * (j-1) + 2] = a->chance[i] + '0';
        break;
      }
    }
  }
  return 1;
}

int ai_removevalues(struct board *b, struct ai *a) {
  for (int i = 0 ; i < NUM_X; i++) {
    for (int j = 0; j < NUM_Y; j++) {
      if (b->coins[i][j] != 0 || j == NUM_Y-1) {
        if (j == NUM_Y-1 && b->coins[i][j] == 0) {
          j++;
        }
        b->visual[i * SIZE_X + 2][SIZE_Y * (j-1) + 1] =
        b->visual[i * SIZE_X + 3][SIZE_Y * (j-1) + 1];
        b->visual[i * SIZE_X + 2][SIZE_Y * (j-1) + 2] =
        b->visual[i * SIZE_X + 3][SIZE_Y * (j-1) + 2];
        break;
      }
    }
  }
  return 1;
}
