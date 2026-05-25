CXX = g++
CXXFLAGS = -fopenmp -O3 -march=native -std=c++17
TARGET = p2_1
SRC = p2_1.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean