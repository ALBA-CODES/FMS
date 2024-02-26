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
		const unsigned threadCount{thread::hardware_concurrency()};
		vector<thread> threads;
		vector<Pool> buff{threadCount};
		atomic<unsigned> index{0};
		bool finish{true};
		mutex mtxSol;
		atomic_int end{0};
		int maxEnd{0};
		solution bestSol;
	public:		
		Consumer(unsigned PTL);
		void execAsync(Node* item);
		void run(unsigned i);
		void finished();
		bool theEnd();
		bool theEnd(solution sol_);
		void setMaxEnd(int maxEnd_);
		void setMaxEnd();
		solution getBestSol();
		int getEnd();
};

#endif
