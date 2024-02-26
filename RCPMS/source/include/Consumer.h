#ifndef __CONSUMER_H__
#define __CONSUMER_H__

#include <memory>
#include <thread>  
#include <vector> 
#include <tuple>      
#include <iostream>
#include <sstream>
#include "Pool.h"

using namespace std;

class Consumer{
	private:
		unsigned threadCount;
		vector<thread> threads;
		vector<Pool> buff;
		atomic<unsigned> index{0};
		int stopInteration;
		atomic<int> bestV{INT_MAX};
		atomic<int> indexPT{0};
		atomic<int> maxDif{0};

		bool finish{true};
		mutex mtxSol;
		atomic_int end{0};
		int maxEnd{0};
		solution bestSol;
	public:		
		Consumer(unsigned PTL, int stopC);
		void execAsync(Node* item);
		void run(unsigned i);
		void finished();
		bool theEnd();
		bool theEnd(solution sol_);
		void setMaxEnd(int maxEnd_);
		void setMaxEnd();
		solution getBestSol();
		void setBestSol(solution b, unsigned i);
		int getEnd();
		atomic<int>* getIndexPT();
		atomic<int>* getMaxDif();
		int getStopC();
};

#endif
