#ifndef GRAPH_H_
#define GRAPH_H_

#include <deque>
#include <map>
#include<queue>
#include <string>
#include <vector>

class Graph {
private:
	
	std::vector<std::vector<unsigned> > nb; // successors
	std::vector<std::vector<unsigned> > pd; // predecessors
	
	

public:
	unsigned n, m;
	std::vector<int> outDegrees;
	std::vector<int> inDegrees;
	std::priority_queue<std::pair<int, int>> maxoutHeap; // 最大堆，pair<出度, 节点>
	Graph(const std::string& filename);
	~Graph();

	const inline std::vector<unsigned>* get_neighbors(unsigned node) const {
		return &nb[node];
	}



	const inline std::vector<unsigned>* get_predecessors(unsigned v) const {
		return &pd[v];
	}


};

Graph::Graph(const std::string& filename) :
	n(0), m(0) {
	std::string line;
	std::ifstream stream(filename.c_str());
	if (!stream.eof()) {
		getline(stream, line);
		std::istringstream iss(line, std::istringstream::in);
		iss >> n >> m;
		nb = std::vector<std::vector<unsigned> >(n);
		pd = std::vector<std::vector<unsigned> >(n);
		outDegrees = std::vector<int>(n,0);
		inDegrees = std::vector<int>(n,0);
		unsigned s, t, d;

		getline(stream, line);
		while (stream.good()) {
			iss.clear();
			iss.str(line);

			while (iss >> s >> t) {
				nb[s].push_back(t);
				pd[t].push_back(s);
				outDegrees[s]++;
				inDegrees[t]++;
			}
			getline(stream, line);
		}

	}
}


Graph::~Graph() {
}
#endif /* GRAPH_H_ */
