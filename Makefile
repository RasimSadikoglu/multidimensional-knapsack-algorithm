make:
	g++ -g -Wall -Wextra -Wpedantic -o main main.cpp
	./main sample1.txt
	./main sample2.txt
	./main sample3.txt