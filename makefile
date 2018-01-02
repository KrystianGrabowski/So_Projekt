projekt.o: projekt.cpp
	g++ -std=c++0x projekt.cpp -o projekt.o

run: projekt.o
	./projekt.o
