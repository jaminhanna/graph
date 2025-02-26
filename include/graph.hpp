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
private:
	int order;
	int size;

	std::vector<Vertex> vertices;
public:
	Graph(const char* filename);

	void add_vertex();

	bool add_edge(
	    int u,
	    int v,
	    int weight = 1,
	    bool directed = false);

	std::vector<int> clique(int k) const;

	bool is_clique(std::vector<int>& v) const;

	std::vector<int> max_clique() const;

	void print() const;
};
