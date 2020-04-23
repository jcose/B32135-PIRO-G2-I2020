pruebaSem: pruebaSem.o Semaphore.o
	g++ -g pruebaSem.o Semaphore.o -o pruebaSem

pruebaSem.o: pruebaSem.cpp
	g++ -c -g pruebaSem.cpp

Semaphore.o: Semaphore.cpp Semaphore.h
	g++ -Wall -c -g Semaphore.cpp -pthread

clean:
	rm -f *.o
