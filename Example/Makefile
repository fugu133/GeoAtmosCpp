CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -pedantic -O2 -I../

all : example

example : Example.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean :
	rm -f example