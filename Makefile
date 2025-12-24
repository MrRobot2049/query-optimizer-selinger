CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

SRC = src/main.cpp src/stats.cpp src/plan.cpp src/cost.cpp \
      src/predicate.cpp src/optimizer.cpp
OUT = optimizer

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
