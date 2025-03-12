#include <cstdio>
#include <iostream>
#include <fstream>
#include <set>
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

void Graph::Kernighan_Lin()
{
	// partition the vertices into two
	// sets a and b

	size_t i, half;
	std::map<int, Vertex*>::iterator it, beg, end;

	i = 0;
	half = vertices.size() / 2;
	beg = vertices.begin();
	end = vertices.end();

	for (it = beg; it != end; ++it) {
		if (i < half) {
			it->second->part = 'a';
			++i;
		} else {
			it->second->part = 'b';
		}
	}

	int gmax;

	gmax = 0;

	do {
		std::set<int> a, b;

		for (it = beg; it != end; ++it) {
			if (it->second->part == 'a') {
				a.insert(it->first);
			} else {
				b.insert(it->first);
			}
		}

		std::set<int>::iterator it2;

		// std::printf("a: {");
		// if (!a.empty()) {
		// 	it2 = a.begin();
		// 	std::printf(" %d", *it2);
		// 	for (++it2; it2 != a.end(); ++it2) {
		// 		std::printf(", %d", *it2);
		// 	}
		// }
		// std::printf(" }\n");

		// std::printf("b: {");
		// if (!b.empty()) {
		// 	it2 = b.begin();
		// 	std::printf(" %d", *it2);
		// 	for (++it2; it2 != b.end(); ++it2) {
		// 		std::printf(", %d", *it2);
		// 	}
		// }
		// std::printf(" }\n");

		int to;
		Vertex* u;
		Vertex* v;
		std::map<int, int>::iterator eit;

		for (it2 = a.begin(); it2 != a.end(); ++it2) {
			v = vertices[*it2];
			eit = v->out.begin();
			for (; eit != v->out.end(); ++eit) {
				to = eit->first;
				if (b.find(to) == b.end()) {
					v->internal.insert(to);
				} else {
					v->external.insert(to);
				}
			}
		}

		for (it2 = b.begin(); it2 != b.end(); ++it2) {
			v = vertices[*it2];
			eit = v->out.begin();
			for (; eit != v->out.end(); ++eit) {
				to = eit->first;
				if (a.find(to) == a.end()) {
					v->internal.insert(to);
				} else {
					v->external.insert(to);
				}
			}
		}

		// for (it = beg; it != end; ++it) {
		// 	v = it->second;

		// 	std::printf("%d: internal: {", v->id);
		// 	if (!v->internal.empty()) {
		// 		it2 = v->internal.begin();
		// 		std::printf(" %d", *it2);
		// 		for (++it2; it2 != v->internal.end(); ++it2) {
		// 			std::printf(", %d", *it2);
		// 		}
		// 	}
		// 	std::printf(" }\n");

		// 	std::printf("%d: external: {", v->id);
		// 	if (!v->external.empty()) {
		// 		it2 = v->external.begin();
		// 		std::printf(" %d", *it2);
		// 		for (++it2; it2 != v->external.end(); ++it2) {
		// 			std::printf(", %d", *it2);
		// 		}
		// 	}
		// 	std::printf(" }\n");
		// }

		std::vector<int> gv, av, bv;

		for (i = 0; i < vertices.size() / 2; ++i) {

			for (it = beg; it != end; ++it) {
				v = it->second;
				v->d = v->external.size() -
				       v->internal.size();
			}

			int max;
			Vertex* x;
			Vertex* y;

			max = 0;
			x = NULL;
			y = NULL;

			it2 = a.begin();
			for (; it2 != a.end(); ++it2) {

				std::set<int>::iterator it3;

				u = vertices[*it2];
				it3 = b.begin();
				for (; it3 != b.end(); ++it3) {

					int g;

					v = vertices[*it3];
					g = u->d + v->d;
					if (u->out.find(v->id) !=
					    u->out.end()) {
						g -= 2;
					}
					if (g > max) {
						max = g;
						x = u;
						y = v;
					}
				}
			}

			if (max < 1) break;

			std::printf("g: %d, u: %d, v: %d\n", max,
			            x->id, y->id);

			a.erase(x->id);
			b.erase(y->id);

			// not sure if things would break on
			// self loops

			for (it = beg; it != end; ++it) {
				v = it->second;
				// if (v == x || v == y) continue;
				v->internal.erase(x->id);
				v->internal.erase(y->id);
				v->external.erase(x->id);
				v->external.erase(y->id);
			}

			gv.push_back(max);
			av.push_back(x->id);
			bv.push_back(y->id);
		}

		if (gv.empty()) {
			std::printf("A: {");
			if (!a.empty()) {
				it2 = a.begin();
				std::printf(" %d", *it2);
				for (++it2; it2 != a.end(); ++it2) {
					std::printf(", %d", *it2);
				}
				std::printf(" }\n");
			}

			std::printf("B: {");
			if (!b.empty()) {
				it2 = b.begin();
				std::printf(" %d", *it2);
				for (++it2; it2 != b.end(); ++it2) {
					std::printf(", %d", *it2);
				}
				std::printf(" }\n");
			}

			return;
		}

		std::set<int> tmp;

		for (i = 0; i < gv.size(); ++i) {
			u = vertices[av[i]];
			v = vertices[bv[i]];
			u->part = 'b';
			v->part = 'a';

			tmp = std::move(u->internal);
			u->internal = std::move(u->external);
			u->external = std::move(tmp);

			tmp = std::move(v->internal);
			v->internal = std::move(v->external);
			v->external = std::move(tmp);
		}
	} while (1);
}
