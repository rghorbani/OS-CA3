all: Process.o Schedulers.o Platform.o Core.o
	g++ -o Sim main.cpp Core.o Schedulers.o Platform.o Process.o
Core.o: Core.cpp Core.h
	g++ -c Core.cpp
Process.o: Process.cpp Process.h
	g++ -c Process.cpp
Platform.o: Platform.cpp Platform.h
	g++ -c Platform.cpp
Schedulers.o: Schedulers.cpp Schedulers.h Comparators.h
	g++ -c Schedulers.cpp
