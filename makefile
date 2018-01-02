projekt.o: projekt.cpp
 	g++ -c projekt.cpp -Wall -fpic

projekt.os: projekt.o
	g++ -o libprojekt.so projekt.o -shared
