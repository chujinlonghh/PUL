#ifndef PUL2H_H
#define PUL2H_H
#include<iostream>
#include <functional>
#include <string>
#include <fstream>
#include <limits>
#include <unordered_set>
#include <queue>
#include <vector>
#include<algorithm>
#include <unordered_map>
#include"roaring.h"
#include"roaring.hh"
#include"roaring64map.hh"



using namespace std;




class TWO_HOP
{
public:
	 roaring::Roaring in;
	 roaring::Roaring out;
	 roaring::Roaring inChao;// assistant, query without, clear all so no size 
	 roaring::Roaring parent;// Mapping supernodes to children
	 TWO_HOP() {

	 };
	 double calculateSize() const {

		 // get all storage bits
		 double totalSize = in.cardinality() + out.cardinality() + parent.cardinality();
		 // Bytes
		 return totalSize/8;
	 }
	
	
};




double calculateMapSizeInMB(vector<TWO_HOP> &index) {
	double totalSize = 0;

	for (const auto& item : index) {
		totalSize += item.calculateSize();
	}

	// MB
	return totalSize / (1024 * 1024);
}



#endif
