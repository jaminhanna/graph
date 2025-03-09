#include <cstdio>
#include <iostream>
#include <fstream>
#include "graph.hpp"

Vertex::Vertex(int id)
    : id(id)
{
	in = 0;
	out = 0;
	deg = 0;
}

void Vertex::add_edge(int v, int weight)
{
	edges.push_back(Edge(v, weight));
}

void Vertex::print() const
{
	std::printf("    %d:", id);

	size_t i;

	for (i = 0; i < edges.size(); ++i) {
		std::printf(" %d", edges[i].v);
	}
	std::printf("\n");
}

Edge::Edge(int v, int weight)
    : v(v),
      weight(weight)
{
}

Graph::Graph(bool directed)
    : directed(directed)
{
	order = 0;
	size = 0;
}

Graph::Graph(const char* filename, bool directed)
    : directed(directed)
{
	order = 0;
	size = 0;

	std::ifstream f(filename);

	if (!f) {
		std::cerr << "failed to open file\n";
		std::exit(1);
	}

	int nnodes, nedges;

	if (!(f >> nnodes >> nedges)) {
		std::cerr << "error parsing file\n";
		std::exit(1);
	}

	int i;

	for (i = 0; i < nnodes; ++i) add_vertex();

	int u, v;

	for (i = 0; i < nedges; ++i) {
		if (!(f >> u >> v)) {
			std::cerr << "error parsing file\n";
			std::exit(1);
		}
		add_edge(u, v);
	}

	f.close();
}

void Graph::add_vertex()
{
	vertices.push_back(Vertex(vertices.size()));
	++order;
}

bool Graph::add_edge(int u, int v, int weight)
{
	if (u < 0 || u >= order) return false;
	if (v < 0 || v >= order) return false;

	vertices[u].add_edge(v, weight);

	++vertices[u].out;
	++vertices[u].deg;

	++vertices[v].in;
	++vertices[v].deg;

	if (!directed) vertices[v].add_edge(u, weight);

	++size;

	return true;
}
void Graph::print() const
{
	std::printf("Graph G:\n");
	std::printf("  Order: %d\n", order);
	std::printf("  Size: %d\n", size);
	std::printf("  Vertices:\n");

	size_t i;

	for (i = 0; i < vertices.size(); ++i) {
		vertices[i].print();
	}
}
