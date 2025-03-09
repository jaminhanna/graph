lib/graph.a: obj/graph.o
	ar rs lib/graph.a obj/graph.o

obj/graph.o: include/graph.hpp src/graph.cpp
	c++ -Iinclude -c src/graph.cpp -o obj/graph.o

clean:
	rm -f bin/* obj/*
