#ifndef Degree_HH
#define Degree_HH

#include "Graph.h"

// Initialize out-degrees and insert them into the max-heap
void initializeOutDegrees(Graph &graph) {

	// Add each node's out-degree to the max-heap
	for (unsigned long i = 0; i < graph.n; ++i) {
		if (graph.outDegrees[i] != 0) {
			graph.maxoutHeap.push({ graph.outDegrees[i], static_cast<int>(i) });
		}
	}
}

// Update the out-degree of a specific node
inline void setOutDegree(Graph &graph, int node, int degree) {
	graph.outDegrees[node] = degree; // Update the out-degree
	graph.maxoutHeap.push({ degree, node }); // Add the new out-degree to the max-heap
}

// Get the current node with the maximum out-degree
inline int getbaseMaxOutDegreeNode(Graph &graph) {
	while (!graph.maxoutHeap.empty()) {
		auto top = graph.maxoutHeap.top();
		graph.maxoutHeap.pop();
		// Check if the out-degree at the heap top matches the value in the out-degrees array
		if (graph.outDegrees[top.second] == top.first) {
			return top.second; // Return the node with the current maximum out-degree
		}
	}
	return -1; // Return -1 if no valid node is found
}

#endif // !Degree
