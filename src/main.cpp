#include <iostream>
#include "graph.hpp"

int main(int argc, char* argv[])
{
	if (argc != 2) {
		std::cerr << "usage: main filename\n";
		std::exit(1);
	}

	Graph g(argv[1]);

	g.max_clique();

	return 0;
}
