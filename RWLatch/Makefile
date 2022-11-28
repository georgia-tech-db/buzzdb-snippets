EXE: RWLatch
	./RWLatch

RWLatch: rwlatch.o 
	g++ -g -o RWLatch rwlatch.o -pthread 

rwlatch.o: rwlatch.cpp rwlatch.hpp
	g++ -g -c -pthread rwlatch.cpp