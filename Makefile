all:
	g++ -c -Wall *.cpp -std=c++0x
	g++ *.o -o go -lfftw3 -lsndlib 

clean:
	rm -f *.o
	rm -f go
