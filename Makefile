# Makefile for estimatePiP2.cpp

CXX = g++
CXXFLAGS = -std=c++11 -pthread
SRCS = estimatePiP2.cpp estimatePiP3.cpp estimatePiP4.cpp estimatePiP5.cpp estimatePiP6.cpp estimatePiP7.cpp estimatePiP8.cpp prefixSumAlgo1.cpp
EXECS = $(SRCS:.cpp=)

.PHONY: all clean

all: $(EXECS)

%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(EXECS)
