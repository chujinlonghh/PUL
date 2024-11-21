#ifndef Graph_Compressd_HH
#define Graph_Compressd_HH

#include "Graph.h"
#include <iostream>
#include <unordered_map>
#include "twoHop.h"
#include "DynamicDegree.h"
#include <algorithm>
#include"roaring.h"
#include"roaring.hh"
#include"roaring64map.hh"


using namespace std;
inline void updateDirect(vector<TWO_HOP>& index, int centralNode, vector<int> &haveCom, int directNode) {

	auto& comIndex = index[directNode].out;  
	for (const int &p : comIndex) { 
		index[p].in.add(centralNode);

	}
}
inline void updatebyHaveCom(vector<TWO_HOP>& index, int centralNode, vector<int> &haveCom, int comNode) {
	auto& cenIndex = index[centralNode].out;  
	auto& comIndex = index[comNode].out;  
	for (const int &p : comIndex) { 
		if (haveCom[p] == 0) {
			cenIndex.add(p);
		}
	}
}

inline void clearHaveCom(vector<TWO_HOP>& index, int centralNode, vector<int> &haveCom) {
	auto& cenIndex = index[centralNode].out; 
	roaring::Roaring toRomve;
	for (const int &p : cenIndex) {
		if (haveCom[p] != 0) {
			toRomve.add(p);
		}
	}
	cenIndex -= toRomve;
}
inline void maintainUnidirect(vector<TWO_HOP> & index, int neighbor, vector<int>& haveCom, int cen) {
	auto &neighborIndex = index[neighbor];  
	auto temp = index[neighbor];

	for (const auto &value : temp.out) {
		auto& outNodes = index[value].out;  
		if (haveCom[value] == 0) {
			updatebyHaveCom(index, neighbor, haveCom, value);
			continue;
		}
		else {
			bool flag = false;
				auto& valIn = index[value].inChao;
				for (const int & it : valIn) {
					if (it != cen) {
						flag = true;
					}
				}
			if (flag) {
				updateDirect(index, value, haveCom, value);
				haveCom[value] = 0;
				roaring::Roaring toRomve;
				for (const int& nod : outNodes) {
					if (haveCom[nod] != 0) {
						toRomve.add(nod);
					}
				}
				outNodes -= toRomve;
			}
			neighborIndex.out |= outNodes;
		}

	}
	

}
inline void updateCentrByCa(vector<TWO_HOP>& index, int centralNode, vector<int> &children) {
	auto& cenIndex = index[centralNode].out;  
	for (const int &p : children) { 
		const auto& outNodes = index[p].out;  
		cenIndex |= outNodes;
		
	}
}

inline void pruneDominate(vector<TWO_HOP>& index, int centralNode, vector<int> &haveCom) {
	auto& cenIndex = index[centralNode].out;  // get address
	roaring::Roaring dominate;
	roaring::Roaring toremove;
	for (const int &p : cenIndex) { 
		if (haveCom[p] == 0) {
			dominate.add(p);
		}
	}
	for (const int &p : cenIndex) { 
		auto& valIn = index[p].inChao;

		for (const int & it : valIn) {
			if (dominate.contains(it)) {
				toremove.add(p);
			}
		}


	}
	cenIndex -= toremove;

}


void graphCompressdPa(int cen, int centralNode, Graph &inputG, vector<TWO_HOP> &index, vector<int>& isCompressed, vector<int>& Compressed, vector<int>& haveCom,int& e) {
	if (isCompressed[centralNode] == 0 || Compressed[centralNode] == 0) {
		return;
	}
	
	//cout << centralNode << " " << inputG.outDegrees[centralNode] << endl;
	
	
	isCompressed[centralNode] = 0;
	e++;

	maintainUnidirect(index, centralNode, haveCom, cen);

	auto & cenIn = index[centralNode].in;
	auto & cenOut = index[centralNode].out;
	bool isP = false;
	bool neiborM = true;
	if (cenIn.isEmpty()) {
		neiborM = false;
	}

	const std::vector<unsigned> *nb_r = inputG.get_predecessors(centralNode);
	for (std::vector<unsigned>::const_iterator it_r = nb_r->begin(); it_r != nb_r->end(); ++it_r) {
		int neighbor = *it_r;  // get out-neighbors

		if (Compressed[neighbor] == 0 || isCompressed[neighbor] == 0) {
			isP = true;
			continue;
		}

		index[neighbor].out.add(centralNode);  // inset neighbors to I_out
		setOutDegree(inputG, neighbor, inputG.outDegrees[neighbor] - 1);
	}
	int nc = 0; 
	vector<int> children;// temporary containers, keeping a record of the child
	bool up = true;

	int n = 0;
	const std::vector<unsigned> *nb = inputG.get_neighbors(centralNode);
	for (std::vector<unsigned>::const_iterator it = nb->begin(); it != nb->end(); ++it) {
		int neighbor = *it;  
		if (Compressed[neighbor] == 0) {

			continue;
		}

		if (isCompressed[neighbor] == 0) {

			continue;
		}

		Compressed[neighbor] = 0;
		nc++;

		children.push_back(neighbor);
		index[neighbor].parent.add(centralNode);
		index[neighbor].inChao.add(cen);
		maintainUnidirect(index, neighbor, haveCom, cen);
		e++;
		// prune hub node
		pruneDominate(index, neighbor, haveCom);
		
		const std::vector<unsigned> *nb_ne = inputG.get_neighbors(neighbor);
		for (std::vector<unsigned>::const_iterator it_n = nb_ne->begin(); it_n != nb_ne->end(); ++it_n) {

			int neighbor_t = *it_n;  // get delete nodes

			if (Compressed[neighbor_t] == 0 || isCompressed[neighbor_t] == 0) {

				continue;
			}
			index[neighbor].out.add(neighbor_t);
			index[neighbor_t].in.add(neighbor);
		}
		const std::vector<unsigned> *nb_rne = inputG.get_predecessors(neighbor);
		for (std::vector<unsigned>::const_iterator it_rn = nb_rne->begin(); it_rn != nb_rne->end(); ++it_rn) {
			int neighbor_s = *it_rn;  // get delete nodes
			if (neighbor_s == centralNode || index[neighbor_s].out.contains(centralNode)) {
				continue;
			}
			bool or_neigh = true;
			if (Compressed[neighbor_s] == 0) {
				continue;
				or_neigh = false;
			}
			if (or_neigh) {
				setOutDegree(inputG, neighbor_s, inputG.outDegrees[neighbor_s] - 1);
			}
			index[neighbor_s].out.add(neighbor);
		
			
		}

	}

	if (inputG.outDegrees[centralNode] != 0) {
		updateCentrByCa(index, centralNode, children);

	}
	if (centralNode != cen) {
		index[centralNode].inChao.add(cen);
		cenIn.add(cen);
	}

	pruneDominate(index, centralNode, haveCom);

}
inline void graphCompressdBa(int CEN, int centralNode, int lastNode, Graph &inputG, vector<TWO_HOP> &index, vector<int>& isCompressed, vector<int>& Compressed, vector<int>& haveCom, double& compressTime,int &e) {
	if (isCompressed[centralNode] == 0 || Compressed[centralNode] == 0) {
		return;
	}
	bool haveNei = false;
	const std::vector<unsigned> *nb = inputG.get_neighbors(centralNode);
	for (std::vector<unsigned>::const_iterator it = nb->begin(); it != nb->end(); ++it) {
		int neighbor = *it;  // get out
		if (isCompressed[neighbor] == 0 || Compressed[neighbor] == 0) {
			continue;
		}
		haveNei = true;
		graphCompressdBa(CEN, neighbor, centralNode, inputG, index, isCompressed, Compressed, haveCom,compressTime,e);
	}
	if (inputG.outDegrees[centralNode] != 0) {
		// compute construct time
		auto start1 = std::chrono::high_resolution_clock::now();
		graphCompressdPa(CEN, centralNode, inputG, index, isCompressed, Compressed, haveCom,e);
		auto end1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> compressTime1 = end1 - start1;
		compressTime += compressTime1.count();
	}
	else if (centralNode == CEN) {
		// compute construct time
		auto start1 = std::chrono::high_resolution_clock::now();
		graphCompressdPa(CEN, centralNode, inputG, index, isCompressed, Compressed, haveCom,e);
		auto end1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> compressTime1 = end1 - start1;
		compressTime += compressTime1.count();
	}


}


void graphCompres(Graph &inputG, vector<TWO_HOP> &index,double& compressTime) {

	std::vector<int> isCompressed(inputG.n, -1);
	std::vector<int> Compressed(inputG.n, -1);
	std::vector<int> haveCom(inputG.n, -1);
	int tree = 0;

	while(!inputG.maxoutHeap.empty()) {
		int centralNode = getbaseMaxOutDegreeNode(inputG);	
		if (isCompressed[centralNode] == 0 || Compressed[centralNode] == 0) {
			continue;
		}
		
		// compute construct time
		auto start1 = std::chrono::high_resolution_clock::now();
		if (inputG.outDegrees[centralNode] == 0) {
	
			//cout << centralNode << endl;
			isCompressed[centralNode] = 0;
			const std::vector<unsigned> *nb1 = inputG.get_neighbors(centralNode);
			auto & curIndex = index[centralNode].out;
			for (std::vector<unsigned>::const_iterator it = nb1->begin(); it != nb1->end(); ++it) {
				curIndex |= index[*it].out;
			}
			auto end3 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> compressTime3 = end3 - start1;
			compressTime += compressTime3.count();
			continue;

		}

		   // Experimental data trc
		    int trc = 0;
		    auto end1 = std::chrono::high_resolution_clock::now();
		    std::chrono::duration<double> compressTime1 = end1 - start1;
		    compressTime += compressTime1.count();
			cout << "seek tree order:" << endl;
			graphCompressdBa(centralNode, centralNode, centralNode, inputG, index, isCompressed, Compressed, haveCom,compressTime,trc);
			haveCom[centralNode] = 0;
			// compute construct time
			auto start2 = std::chrono::high_resolution_clock::now();
			// prune
			clearHaveCom(index, centralNode, haveCom);
			auto end2 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> compressTime2 = end2 - start2;
			compressTime += compressTime2.count();
			if (tree <= trc) {
				tree = trc;
			}
	}
	/*
	cout <<"tree:"<< tree << endl;
	int hub = 0;
	for (int i = 0; i < inputG.n; i++) {
		if (haveCom[i] == 0) {
			hub++;
		}
	}
	cout << "hub:" << hub << endl;
	*/
}

#endif



