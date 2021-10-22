// Copyright 2019 Jakob Sailer
// Author: Jakob Sailer <sailer.jakob@web.de>

#include <assert.h>

#include "./saveload.h"

int main(int argc, char * argv[]) {
  printf("Trying to parse savefile from file %s...\n", argv[1]);
  bool b = parse_save(argv[1]);
  printf(b ? "success\n\n" : "failed\n\n");
  return 0;
}
