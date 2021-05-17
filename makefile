
#  Created by Simeon Lico (z1885981)

.DEFAULT_GOAL := assign4

assign4.o: assign4.cc
	g++ -c assign4.cc -Wall -std=c++11

assign4: assign4.o
	g++ -pthread -o assign4 assign4.o -Wall -std=c++11

clean:
	rm *.o assign4