all:
	g++ -c -Wall *.cpp -std=c++0x
	g++ *.o -o go -lfftw3 -lsndlib -lboost_serialization -lboost_filesystem -lboost_system

clean:
	rm -f *.o
	rm -f go
