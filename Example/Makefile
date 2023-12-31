CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -pedantic -O3 -I../

all : example

example : CheckCoeff.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean :
	rm -f example