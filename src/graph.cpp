#include <cstdio>
#include <iostream>
#include <fstream>
#include <set>
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

// this method performs an n choose k to
//   determine if there is any clique of
//   size k
// it treats a vector like a stack to
//   avoid explicit recursion

std::vector<int> Graph::clique(int k) const
{
	size_t i;
	int e, index;
	int need, left;
	std::vector<int> v;

	v.push_back(-1);

	while (!v.empty()) {

		// for (i = 0; i < v.size(); ++i) {
		// 	std::printf(" %d", v[i]);
		// }
		// std::printf("\n");

		e = v.back();
		v.pop_back();

		if (v.size() == k) {
			if (is_clique(v)) return v;
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

	return v;
}

// return whether every vertex in v is
//   connected to every other vertex in v

bool Graph::is_clique(std::vector<int>& v) const
{
	size_t i, j;
	std::set<int> s, s2;

	// std::printf("is_clique( {");
	// std::printf(" %d", v[0]);
	// for (i = 1; i < v.size(); ++i) std::printf(", %d", v[i]);
	// std::printf(" } )\n");

	for (i = 0; i < v.size(); ++i) s.insert(v[i]);

	std::set<int>::const_iterator it;

	for (i = 0; i < v.size(); ++i) {

		s.erase(v[i]);

		for (j = 0; j < vertices[i].edges.size(); ++j) {
			s2.insert(vertices[i].edges[j].v);
		}

		// std::printf("s: %lu, s2: %lu\n", s.size(),
		//     s2.size());

		for (it = s.begin(); it != s.end(); ++it) {
			if (s2.find(*it) == s2.end()) return false;
		}

		s.insert(v[i]);
		s2.clear();
	}

	return true;
}

// since we know, if there is a clique of
//   size k, that there is a clique of size
//   k - 1, and also that, if there is no
//   clique of size k, then there is no
//   clique of size k + 1, we can use
//   binary search to find the largest
//   value of k such that there is a clique
//   of size k, in logarithmic time

std::vector<int> Graph::max_clique() const
{
	int start;
	int size;
	int k;
	std::vector<int> v;

	// handle the cases where |V| < 2

	if (vertices.empty()) return v;

	if (vertices.size() == 1) return std::vector<int>(1, 0);

	start = 1;
	size = vertices.size();

	while (size > 1) {
		k = start + size / 2;

		// std::printf("%d %d %d\n", start, k,
		//     start + size - 1);

		v = clique(k);

		// if v is not empty then there was
		//   a clique of size k
		// we therefore look for a clique of
		//   size more than k
		// otherwise we know there is no
		//   clique of size more than k, so
		//   we look for a smaller one

		if (!v.empty()) {
			start += size / 2;
			size -= size / 2;
		} else {
			size /= 2;
		}
	}

	return v;
}

bool Graph::is_vertex_cover(std::vector<int>& v) const
{
	int count;

	count = 0;

	size_t i;

	for (i = 0; i < v.size(); ++i) {
		count += vertices[v[i]].edges.size();
	}

	// if (!directed) count /= 2;
	count /= 2; // assume undirected

	return count == size;
}

std::vector<int> Graph::vertex_cover(int k) const
{
	size_t i;
	int e, index;
	int need, left;
	std::vector<int> v;

	v.push_back(-1);

	while (!v.empty()) {

		// for (i = 0; i < v.size(); ++i) {
		// 	std::printf(" %d", v[i]);
		// }
		// std::printf("\n");

		e = v.back();
		v.pop_back();

		if (v.size() == k) {
			if (is_vertex_cover(v)) return v;
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

	return v;
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
