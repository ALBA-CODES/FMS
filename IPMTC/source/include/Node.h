#ifndef __NODE_H__
#define __NODE_H__

#include <vector>
#include <memory>
#include <cmath> 
#include <mutex>
#include <atomic>
#include <algorithm>
#include <iostream>
#include "IPMTC.h"

using namespace std;

class Node{
	//private:
		
	protected:
		atomic<int>* execMax;
		mutex mtxNode;
		int execAtual = 0;
		bool endN = false; 
		bool fase2 = false; 
		bool MCMCType = false; 
		atomic<int>* indexPT;
		int stopC_;
		vector<std::pair<Node*,bool>> edgeFrom;
		vector<std::pair<Node*,bool>> edgeto;
		solution sol;
		solution bestSol;
		IPMTC* prob;
		std::chrono::high_resolution_clock::time_point startTime;

	public:
		Node();
		~Node();
		virtual void run() = 0;
		virtual bool ready() = 0;
		virtual bool notify() = 0;
		virtual bool observer(Node* from) = 0;
		bool addEdge(Node* from, Node* to);
		bool theEnd();	
		bool finish();
		void reset();
		void printEdgeto();
		void printEdgeFrom();
};

#endif
