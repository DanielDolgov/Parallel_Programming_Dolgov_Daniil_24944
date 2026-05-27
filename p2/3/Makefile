CXX = g++
CXXFLAGS = -fopenmp -O3 -march=native -std=c++17
TARGETS = p2_3 v1 v2

all: $(TARGETS)

p2_3: p2_3.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

v1: v1.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

v2: v2.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean