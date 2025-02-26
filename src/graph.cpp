#include <cstdio>
#include <iostream>
#include <fstream>
#include <stack>
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

// returns whether the graph contains a
// clique of size k

bool Graph::clique(int k) const
{
	size_t i;
	int e, index;
	int need, left;
	std::vector<int> v;

	v.push_back(-1);

	while (!v.empty()) {

		// for (i = 0; i < v.size(); ++i) std::printf(" %d", v[i]);
		// std::printf("\n");

		e = v.back();
		v.pop_back();

		if (v.size() == k) {
			if (is_clique(v)) return true;
		} else if (e == -1) {
			if (v.empty()) v.push_back(0);
			else v.push_back(v.back() + 1);
			v.push_back(-1);
		} else if (e + 1 < vertices.size()) {
			need = k - (v.size() + 1);
			left = vertices.size() - (e + 1) - 1;

			if (need > left) continue;

			v.push_back(e + 1);
			v.push_back(-1);
		}
	}

	return false;
}

bool Graph::is_clique(std::vector<int>& v) const
{
	size_t i;

	for (i = 0; i < v.size(); ++i) std::printf(" %d", v[i]);
	std::printf("\n");

	return false;
}

// returns a vector of vertices comprising
// a maximum clique in the graph

std::vector<int> Graph::max_clique() const
{
	int start;
	int size;
	int k;

	start = 0;
	size = vertices.size();

	while (size > 1) {
		k = start + size / 2;
		// std::printf("\t%d\t%d\t%d\n", start, k, start+size-1);
		// std::printf("clique(k): %d\n", (int)clique(k));

		if (clique(k)) {
			start += size / 2;
			size -= size / 2;
		} else {
			size /= 2;
		}
	}

	return std::vector<int>();
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
