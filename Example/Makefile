CXX := g++
CXXFLAGS := -std=c++2a -Wall -Wextra -Werror -pedantic -O2 -I../

all : ccadm ccada crsd dlswdata

ccadm : CheckCalcAtmosDensManu.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

ccada : CheckCalcAtmosDensAuto.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

crsd : CheckReadSwDataset.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean :
	rm -f ccadm ccada crsd atmos.csv SW-Last5Years.csv

dlswdata:
	python3 DlSwDataset.py