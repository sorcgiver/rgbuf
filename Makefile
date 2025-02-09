
CXX = g++
CC = gcc

RGPATH = ./src
RGCORE_PATH = ./src/core
EXAMPLE_PATH = ./examples
TEST_PATH = ./tests

CFLAGS = -I$(RGPATH) -Wall -Wextra
CXXFLAGS = -I$(RGPATH) -Wall -Wextra

all: example test

example: $(EXAMPLE_PATH)/example.o $(RGCORE_PATH)/rgbase.o $(RGPATH)/rgbuf.o
	$(CXX) -o $@ $^

test: $(TEST_PATH)/test.o $(RGCORE_PATH)/rgbase.o $(RGPATH)/rgbuf.o $(RGPATH)/rgpbuf.o
	$(CXX) -o $@ $^

$(TEST_PATH)/test.o: $(TEST_PATH)/test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

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
	rm -rf vgcore.*
