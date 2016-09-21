CXX = g++
CXXFLAGS = -Wall -std=c++11 -g
TARGETS = server client

all: $(TARGETS)
%: %.cpp
	$(CXX) $^ $(CXXFLAGS) -o $@

.PHONY: clean
clean:
	-rm -rf $(TARGETS) *.dSYM
