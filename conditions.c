// Copyright 2019 Jakob Sailer
// Author: Jakob Sailer <sailer.jakob@web.de>

#include "./board.h"
#include "./conditions.h"

int add_coin(struct board *b, int x, int ply) {
  if (b->coins[x][0] != 0) {
    // whole column is full
    return 0;
  }
  // find the lowest row to place the coin
  for (int i = 0; i < NUM_Y; i++) {
    if (b->coins[x][i + 1] != 0 || i + 1 == NUM_Y) {
      // add a coin of the correct player to the coinmatrix
      // and save this moves coords
      print_ply(b, x * SIZE_X, i * SIZE_Y, ply);
      b->lastmove[0] = x;
      b->lastmove[1] = i;
      break;
    }
  }
  return 1;
}

int check_vertical(struct board *b, int x, int y, int ply) {
  int checksum = 1, i = 1;
  if (!b->coins[x][y] || b->coins[x][y] != ply) {
    // if the current coin is 0 or not of the specified player color
    return 0;
  }
  // check up
  while (b->coins[x][y - i] == ply) {
    checksum++;
    i++;
  }
  i = 1;
  // check down
  while (b->coins[x][y + i] == ply) {
    checksum++;
    i++;
  }
  return checksum;
}

int check_horizontal(struct board *b, int x, int y, int ply) {
  int checksum = 1, i = 1;
  if (!b->coins[x][y] || b->coins[x][y] != ply) {
    // if the current coin is 0 or not of the specified player color
    return 0;
  }
  // check right
  while (b->coins[x + i][y] == ply) {
    checksum++;
    i++;
  }
  i = 1;
  // check left
  while (b->coins[x - i][y] == ply) {
    checksum++;
    i++;
  }
  return checksum;
}

int check_decline(struct board *b, int x, int y, int ply) {
  int checksum = 1, i = 1;
  if (!b->coins[x][y] || b->coins[x][y] != ply) {
    // if the current coin is 0 or not of the specified player color
    return 0;
  }
  // check right down
  while (b->coins[x + i][y + i] == ply) {
    checksum++;
    i++;
  }
  i = 1;
  // check left up
  while (b->coins[x - i][y - i] == ply) {
    checksum++;
    i++;
  }
  return checksum;
}

int check_incline(struct board *b, int x, int y, int ply) {
  int checksum = 1, i = 1;
  if (!b->coins[x][y] || b->coins[x][y] != ply) {
    // if the current coin is 0 or not of the specified player color
    return 0;
  }
  // right up
  while (b->coins[x + i][y - i] == ply) {
    checksum++;
    i++;
  }
  i = 1;
  // check left down
  while (b->coins[x - i][y + i] == ply) {
    checksum++;
    i++;
  }
  return checksum;
}

int check_game(struct board *b) {
  // only the corresponding rows and columns
  // of the last move are relevant for game termination
  int ply = b->coins[b->lastmove[0]][b->lastmove[1]];
  int x = b->lastmove[0];
  int y = b->lastmove[1];

  // check vertical
  if (check_vertical(b, x, y, ply) >= 4) {
    return 1;
  }

  // check horizontal
  if (check_horizontal(b, x, y, ply) >= 4) {
    return 1;
  }

  // check diagonal decline
  if (check_decline(b, x, y, ply) >= 4) {
    return 1;
  }

  // check diagonal incline
  if (check_incline(b, x, y, ply) >= 4) {
    return 1;
  }
  // if none of the above tests returned true
  // no player has won yet
  return 0;
}
