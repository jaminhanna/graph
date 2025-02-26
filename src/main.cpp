#include <iostream>
#include "graph.hpp"

int main(int argc, char* argv[])
{
	if (argc != 2) {
		std::cerr << "usage: main filename\n";
		std::exit(1);
	}

	Graph g(argv[1]);

	std::vector<int> v;

	// g.print();

	v = g.max_clique();

	std::cout << "Size of maximum clique: " << v.size() << '\n';

	return 0;
}
