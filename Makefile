# Copyright 2019 Jakob Sailer
# Author: Jakob Sailer <sailer.jakob@web.de>
# with snippets provided by https://proglang.informatik.uni-freiburg.de/teaching/advanced-programming/2019/

CC = gcc
CFLAGS = --std=c11 -pedantic -Wall
BINARIES = saveloadtest boardtest frametest conditionstest aitest 4

.PHONY: all clean compile test checkstyle

# all
all: checkstyle compile test

# delete
clean:
	rm -f $(BINARIES) *.o

# checkstyle
checkstyle:
	python3 ../cpplint3.py --filter=-runtime/int,-readability/casting,-runtime/threadsafe_fn,-runtime/printf --repository=. *.h *.c

# compile
compile: $(BINARIES)

board.o: board.c board.h
	$(CC) $(CFLAGS) board.c -c

saveload.o: saveload.c frame.o saveload.h
	$(CC) $(CFLAGS) saveload.c -c

frame.o: board.o frame.c frame.h
	$(CC) $(CFLAGS) frame.c -c

conditions.o: board.o conditions.c conditions.h
	$(CC) $(CFLAGS) conditions.c -c

ai.o: conditions.o ai.c ai.h
	$(CC) $(CFLAGS) ai.c -c

boardtest: board.o boardtest.c
	$(CC) $(CFLAGS) board.o frame.o saveload.o boardtest.c -o boardtest

# tests
saveloadtest: saveload.o saveloadtest.c
	$(CC) $(CFLAGS) saveload.o frame.o saveloadtest.c -o saveloadtest
	
frametest: frame.o frametest.c
	$(CC) $(CFLAGS) board.o saveload.o frame.o frametest.c -o frametest

conditionstest: conditions.o conditionstest.c
	$(CC) $(CFLAGS) board.o frame.o saveload.o conditions.o conditionstest.c -o conditionstest
  
aitest: ai.o aitest.c
	$(CC) $(CFLAGS) ai.o board.o frame.o conditions.o saveload.o aitest.c -o aitest

# executable main
4: frame.o conditions.o ai.o saveload.o 4.c
	$(CC) $(CFLAGS) board.o frame.o conditions.o ai.o saveload.o 4.c -o 4

# tests
test:
	./saveloadtest save1.4
	./saveloadtest save2.4
	./saveloadtest save3.4
	# the following save tests should fail
	./saveloadtest save4.4
	./saveloadtest save5.4
	./saveloadtest save6.4
	./saveloadtest save7.4
	./saveloadtest save8.4
	./boardtest
	./frametest
	./conditionstest
	./aitest
