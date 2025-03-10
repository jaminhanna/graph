#include <cstdio>
#include <iostream>
#include <fstream>
#include "graph.hpp"

Vertex::Vertex(int id)
    : id(id)
{
}

void Vertex::print() const
{
	std::printf("    %d:", id);

	std::map<int, int>::const_iterator it, beg, end;

	beg = out.begin();
	end = out.end();

	for (it = beg; it != end; ++it) {
		std::printf(" %d", it->first);
	}
	std::printf("\n");
}

// Edge::Edge(int v, int weight)
//     : v(v),
//       weight(weight)
// {
// }

Graph::Graph(bool directed)
    : directed(directed)
{
}

Graph::Graph(const char* filename, bool directed)
    : directed(directed)
{
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

	for (i = 0; i < nnodes; ++i) add_vertex(i);

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

Graph::~Graph()
{
	std::map<int, Vertex*>::iterator it, beg, end;

	beg = vertices.begin();
	end = vertices.end();

	for (it = beg; it != end; ++it) delete it->second;
}

bool Graph::add_vertex(int id)
{
	if (vertices.find(id) != vertices.end()) return false;

	vertices[id] = new Vertex(id);

	return true;
}

bool Graph::add_edge(int u, int v, int weight)
{
	if (vertices.find(u) == vertices.end()) return false;
	if (vertices.find(v) == vertices.end()) return false;

	Vertex* a;
	Vertex* b;

	a = vertices.find(u)->second;
	b = vertices.find(v)->second;

	a->out[v] = weight;
	b->in[u] = weight;

	if (!directed) {
		b->out[u] = weight;
		a->in[v] = weight;
	}

	return true;
}
void Graph::print() const
{
	std::printf("Graph G:\n");
	std::printf("  Vertices:\n");

	std::map<int, Vertex*>::const_iterator it, beg, end;

	beg = vertices.begin();
	end = vertices.end();

	for (it = beg; it != end; ++it) it->second->print();
}
