CXX=clang++ -std=c++11
CXXFLAGS=-Wall -O2

edit : buffer.o editor.o grammar.o main.o regexp.o
	$(CXX) buffer.o editor.o grammar.o main.o regexp.o -o $@

buffer.o : edit.hpp buffer.cpp
	$(CXX) $(CXXFLAGS) -c buffer.cpp -o $@

editor.o : edit.hpp editor.cpp
	$(CXX) $(CXXFLAGS) -c editor.cpp -o $@

grammar.o : edit.hpp grammar.cpp
	$(CXX) $(CXXFLAGS) -c grammar.cpp -o $@

main.o : edit.hpp main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@

regexp.o : edit.hpp regexp.cpp
	$(CXX) $(CXXFLAGS) -c regexp.cpp -o $@

clean :
	rm -f *.o edit tests/t-buffer tests/t-grammar tests/t-regexp tests/t-editor

prove : tests/t-buffer tests/t-grammar tests/t-regexp tests/t-editor
	prove ./tests/t-buffer ./tests/t-grammar ./tests/t-regexp ./tests/t-editor

test : tests/t-buffer tests/t-grammar tests/t-regexp tests/t-editor
	./tests/t-buffer
	./tests/t-grammar
	./tests/t-regexp
	./tests/t-editor

tests/t-buffer : edit.hpp tests/t-buffer.cpp buffer.o
	$(CXX) -I . -I ./tests tests/t-buffer.cpp buffer.o -o $@

tests/t-grammar : edit.hpp tests/t-grammar.cpp grammar.o
	$(CXX) -I . -I ./tests tests/t-grammar.cpp grammar.o -o $@

tests/t-regexp : edit.hpp tests/t-regexp.cpp regexp.o
	$(CXX) -I . -I ./tests tests/t-regexp.cpp regexp.o -o $@

tests/t-editor : edit.hpp tests/t-editor.cpp buffer.o editor.o grammar.o regexp.o
	$(CXX) -I . -I ./tests tests/t-editor.cpp buffer.o editor.o grammar.o regexp.o -o $@
