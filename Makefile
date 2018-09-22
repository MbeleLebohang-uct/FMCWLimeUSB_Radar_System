#
# File:   Makefile for FMCW Radar System running on LimeSDR
# Author: Mbele Lebohang
# 
# Created on 04 May 2017, 09:22
#

# Variables

CC = g++
CCFLAGS = -Wextra -pedantic -std=c++11
SOURCES = $(wildcard src/*.cpp)
SOURCES_TEST = $(wildcard tests/*.cpp)
OBJ = $(SOURCES:src/%.cpp=bin/%.o)
OBJ_TEST = $(filter-out bin/main.o, $(OBJ)) $(SOURCES_TEST:tests/%.cpp=bin/%.o)
LIBS = -lm -lpthread $(wildcard /usr/local/lib/libLimeSuite.so)
INC = -I headers
INC_TEST = -I lib/catch/inc


#---------------------------------------------------------------------------------
.PHONY: all
all: fmcwradar
fmcwradar: $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) -o fmcwradar $(LIBS)

#---------------------------------------------------------------------------------
bin/%.o: src/%.cpp
	$(CC) $(CCFLAGS) -c $< $(INC) -o $@

#---------------------------------------------------------------------------------
.PHONY: clean
clean:
	rm -f bin/*.o 
	rm  ./fmcwradar