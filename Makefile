all:
	g++ -c -Wall *.cpp
	g++ *.o -o go -lfftw3 -lsndlib

clean:
	rm -f *.o
	rm -f go