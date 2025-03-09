#include <vector>

class Edge;
class Graph;

class Vertex {
	friend class Graph;
private:
	int id;

	int in;
	int out;
	int deg;

	std::vector<Edge> edges;
public:
	Vertex(int id);

	void add_edge(int v, int weight);

	void print() const;
};

class Edge {
	friend class Vertex;
	friend class Graph;
private:
	int v;
	int weight;
public:
	Edge(int v, int weight = 1);
};

class Graph {
public: // temporary
// private:
	int order;
	int size;
	const bool directed;

	std::vector<Vertex> vertices;
public:
	Graph(bool directed = false);
	Graph(const char* filename, bool directed = false);

	void add_vertex();

	bool add_edge(int u, int v, int weight = 1);

	void print() const;
};
