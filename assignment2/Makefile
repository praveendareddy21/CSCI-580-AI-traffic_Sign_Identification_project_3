G = g++
GFLAGS = -g -std=c++11 -Wall -W -Werror -pedantic

ann: ann.o main.o
	$(G) $(GFLAGS) main.o ann.o -o ann

main.o: main.cpp ann.h

ann.o: ann.cpp ann.h

clean:
	rm -f *.o ann

tests:
	echo "feature not yet added"
