#ifndef __CONSUMER_H__
#define __CONSUMER_H__

#include <memory>
#include <thread>  
#include <vector> 
#include <tuple>
#include <climits>      
#include <iostream>
#include <sstream>
#include "Pool.h"
#include "ExecTime.h"

using namespace std;

class Consumer{
	private:
		unsigned threadCount;
		vector<thread> threads;
		vector<Pool> buff;
		atomic<unsigned> index{0};
		int stopC_;
		atomic<int> bestV{INT_MAX};
		atomic<int> indexPT{0};
		ExecTime* timeEnd;
		
		bool finish{true};
		mutex mtxSol;
		atomic_int end{0};
		int maxEnd{0};
		solution bestSol;
	public:		
		Consumer(unsigned PTL, int stopC, ExecTime* time);
		void execAsync(Node* item);
		void run(unsigned i);
		void finished();
		bool theEnd();
		bool theEnd(solution sol_);
		void setMaxEnd(int maxEnd_);
		void setMaxEnd();
		solution getBestSol();
		void setBestSol(solution b, unsigned i, bool fase);
		atomic<int>* getIndexPT();
		int getStopC();
		int getEnd();
		std::chrono::high_resolution_clock::time_point getStart();
};

#endif
