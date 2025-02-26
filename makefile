bin/main: obj/graph.o obj/main.o
	c++ obj/graph.o obj/main.o -o bin/main
obj/graph.o: include/graph.hpp src/graph.cpp
	c++ -Iinclude -c src/graph.cpp -o obj/graph.o
obj/main.o: include/graph.hpp src/main.cpp
	c++ -Iinclude -c src/main.cpp -o obj/main.o

clean:
	rm -f bin/* obj/*
