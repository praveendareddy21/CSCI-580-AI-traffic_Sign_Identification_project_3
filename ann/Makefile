#G = g++
G = clang++
LIBS = -L/usr/local/opt/openblas/lib/ -I /usr/local/opt/openblas/include/
FLAGS = -Ofast -std=c++11 -stdlib=libc++ -Wall -W -Werror -pedantic

ann: ann.o main.o
	$(G) $(FLAGS) -o ann ann.o main.o

ann.o: ann.cpp ann.h

main.o: main.cpp ann.h

clean:
	rm -f *.o ann

test1:
	make && runtest 1

test2:
	make && runtest 2

test3:
	make && runtest 3

test4:
	make && runtest 4

testall:
	make && runall
