
CXX = g++
CC = gcc

SRC = ./src
RGPATH = ./src/static
RGCORE_PATH = ./src/core
EXAMPLE_PATH = ./examples/static
EXAMPLE_BIN = ./examples
TEST_PATH = ./tests

CFLAGS = -I$(SRC) -I$(RGPATH) -Wall -Wextra
CXXFLAGS = -I$(SRC) -I$(RGPATH) -Wall -Wextra

all: $(EXAMPLE_BIN)/example_rgbuf $(EXAMPLE_BIN)/example_rgpbuf test

$(EXAMPLE_BIN)/example_rgbuf: $(EXAMPLE_PATH)/example_rgbuf.o $(RGCORE_PATH)/rgbase.o $(RGPATH)/rgbuf.o $(RGPATH)/rgpbuf.o
	$(CC) -o $@ $^

$(EXAMPLE_BIN)/example_rgpbuf: $(EXAMPLE_PATH)/example_rgpbuf.o $(RGCORE_PATH)/rgbase.o $(RGPATH)/rgbuf.o $(RGPATH)/rgpbuf.o
	$(CC) -o $@ $^

test: $(RGCORE_PATH)/rgbase.o $(RGPATH)/rgbuf.o $(RGPATH)/rgpbuf.o $(TEST_PATH)/test.o
	$(CXX) -o $@ $^

$(TEST_PATH)/test.o: $(TEST_PATH)/test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXAMPLE_PATH)/example_rgbuf.o: $(EXAMPLE_PATH)/example_rgbuf.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXAMPLE_PATH)/example_rgpbuf.o: $(EXAMPLE_PATH)/example_rgpbuf.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXAMPLE_PATH)/example.o: $(EXAMPLE_PATH)/example.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(RGCORE_PATH)/rgbase.o: $(RGCORE_PATH)/rgbase.c $(RGCORE_PATH)/rgbase.h
	$(CC) $(CFLAGS) -c $< -o $@

$(RGPATH)/rgbuf.o: $(RGPATH)/rgbuf.c $(RGPATH)/rgbuf.h
	$(CC) $(CFLAGS) -c $< -o $@

$(RGPATH)/rgpbuf.o: $(RGPATH)/rgpbuf.c $(RGPATH)/rgpbuf.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(RGPATH)/*.o
	rm -rf $(RGCORE_PATH)/*.o
	rm -rf $(EXAMPLE_PATH)/*.o
	rm -rf $(TEST_PATH)/*.o
	rm -rf test
	rm -rf example
	rm -rf $(EXAMPLE_BIN)/example_rgbuf
	rm -rf $(EXAMPLE_BIN)/example_rgpbuf
	rm -rf vgcore.*
