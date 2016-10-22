G = g++
GFLAGS = -g -std=c++11 -Wall -W -Werror -pedantic

robot : matrix.o main.o robot.o
	$(G) $(GFLAGS) matrix.o main.o robot.o -o robot

main.o: main.cpp robot.h matrix.h

robot.o: robot.cpp robot.h

matrix.o: matrix.cpp matrix.h

clean:
	rm -f *.o robot
