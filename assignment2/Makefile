G = g++
GFLAGS = -g -std=c++11 -Wall -W -Werror -pedantic

ann: ann.o main.o
	$(G) $(GFLAGS) main.o ann.o -o ann

main.o: main.cpp ann.h

ann.o: ann.cpp ann.h

clean:
	rm -f *.o ann

tests:
	./ann train_input2.txt train_output2.txt test_input2.txt test_output2.txt structure2.txt weights2.txt 100 > myout1 && diff myout1 t01.out;
	./ann train_input2.txt train_output2.txt test_input2.txt test_output2.txt structure2.txt weights2.txt 700 > myout2 && diff myout2 t02.out;
	./ann train_input2.txt train_output2.txt test_input2.txt test_output2.txt structure2.txt weights2.txt 300 > myout3 && diff myout3 t03.out;
	./ann train_input2.txt train_output2.txt test_input2.txt test_output2.txt structure3.txt weights3.txt 100 > myout4 && diff myout4 t04.out;

test1:
	make && ./ann train_input2.txt train_output2.txt test_input2.txt test_output2.txt structure2.txt weights2.txt 100

test2:
	make && ./ann train_input2.txt train_output2.txt test_input2.txt test_output2.txt structure2.txt weights2.txt 700

test3:
	make && ./ann train_input2.txt train_output2.txt test_input2.txt test_output2.txt structure2.txt weights2.txt 300

test4:
	make && ./ann train_input2.txt train_output2.txt test_input2.txt test_output2.txt structure3.txt weights3.txt 100
