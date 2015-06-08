CXX=g++ -std=c++11
CXXFLAGS=-Wall -O2

edit : buffer.o editor.o grammar.o main.o regexp.o
	$(CXX) -o edit buffer.o editor.o grammar.o main.o regexp.o

buffer.o: edit.hpp buffer.cpp
	$(CXX) $(CXXFLAGS) -c buffer.cpp

editor.o: edit.hpp editor.cpp
	$(CXX) $(CXXFLAGS) -c editor.cpp

grammar.o: edit.hpp grammar.cpp
	$(CXX) $(CXXFLAGS) -c grammar.cpp

main.o: edit.hpp main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

regexp.o: edit.hpp regexp.cpp
	$(CXX) $(CXXFLAGS) -c regexp.cpp

clean:
	rm -f *.o edit
