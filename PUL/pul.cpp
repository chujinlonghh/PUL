#include <iostream>
#include <string>
#include <random>
#include <fstream>
#include <sstream>
#include <chrono>
#include <malloc.h>
#include"Graph.h"
#include"twoHop.h"
#include"graphCompressd.h"
#include"Cycle.h"
#include"Query.h"
#include"roaring.hh"
#include"roaring.c"






int main()
{
	Graph inputG("inputGraph.txt");
	

	vector<TWO_HOP> index(inputG.n);
	initializeOutDegrees(inputG);
	// Record compression time
	double compressTime = 0.0;
	auto start1 = std::chrono::high_resolution_clock::now();
	graphCompres(inputG, index,compressTime);
	auto end1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> allTime = end1 - start1;

	cout << "Seek Tree time: " << allTime.count()- compressTime << endl;

	double size_1 = calculateMapSizeInMB(index);

	cout << "Construct time: " << compressTime << endl;
	cout << "Index size: " << size_1<<endl;

	
	
	std::string line;
	std::ifstream inputFile("queryTest.txt");
	if (!inputFile.is_open()) {
		std::cout << "Failed to open input file." << std::endl;
		return 1;
	}
	int yes = 0;
	int no = 0;
	auto start3 = std::chrono::high_resolution_clock::now();
	while (std::getline(inputFile, line)) {
		std::istringstream iss(line);

		int first, second;
		if (!(iss >> first >> second)) {
			continue;
		}
		auto start2 = std::chrono::high_resolution_clock::now();
		if (query(index, first, second)) {
			yes++;
		}	
	}
	auto end3 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> queryTime = end3 - start3;
	cout << "Query time: " << queryTime.count() << endl;
	cout << yes << " pairs reachable" << endl;
	
	return 0;
}
