#ifndef __NODE_H__
#define __NODE_H__

#include <vector>
#include <memory>
#include <cmath> 
#include <mutex>
#include <algorithm>
#include <iostream>
#include "RCPMS.h"

using namespace std;

class Node{
	//private:
		
	protected:
		atomic<int>* execMax;
		mutex mtxNode;
		int execAtual = 0; 
		bool endN = false; 
		atomic<int>* indexPT;
		int stopInt;
		vector<std::pair<Node*,bool>> edgeFrom;
		vector<std::pair<Node*,bool>> edgeto;
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
