all: compile

compile: test-reversi.cpp reversi.cpp
	g++ reversi.cpp test-reversi.cpp -o test-reversi -g

memchk: test-reversi
	valgrind --tool=memcheck --leak-check=yes ./test-reversi

clean:
	rm ./test-reversi