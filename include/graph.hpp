#include <vector>
#include <map>
#include <set>

class Edge;
class Graph;

class Vertex {
public:
	const int id;

	std::map<int, int> in;
	std::map<int, int> out;

	// Kernighan-Lin

	int d;
	std::set<int> internal;
	std::set<int> external;

	// Kernighan-Lin

	Vertex(int id);

	void print() const;
};

// class Edge {
// public:
// 	int v;
// 	int weight;
// 
// 	Edge(int v, int weight = 1);
// };

class Graph {
public:
	const bool directed;

	std::map<int, Vertex*> vertices;

	Graph(bool directed = false);
	Graph(const char* filename, bool directed = false);

	~Graph();

	bool add_vertex(int id);

	bool add_edge(int u, int v, int weight = 1);

	void Kernighan_Lin();

	void print() const;
};
