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
	printf("\n");
	printf("  Vertex %d:", id);

	size_t i;

	for (i = 0; i < edges.size(); ++i) {
		printf(" %d", edges[i].v);
	}
	printf("\n");
}

Edge::Edge(int v, int weight)
    : v(v),
      weight(weight)
{
}

Graph::Graph(const char* filename)
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

bool Graph::add_edge(
    int u,
    int v,
    int weight,
    bool directed)
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
	printf("Graph G:\n");
	printf("  Order: %d\n", order);
	printf("  Size: %d\n", size);

	size_t i;

	for (i = 0; i < vertices.size(); ++i) {
		vertices[i].print();
	}
}
