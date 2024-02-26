#ifndef __RCPMS_H__
#define __RCPMS_H__

#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <utility>
#include <forward_list>
#include <list>
#include <set>
#include <map>
#include <omp.h>
#include <limits.h>
#include <random>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <thread>
#include <atomic>
#include "Problem.h"


class RCPMS: public Problem{
	private:
		int numberJobs;
		int numberTools;
		int numberMachine;
		int changeTax;
		int nType_;
		int moveF;
		int meio;
		std::vector<int>  JobTools;
		std::vector<int> JobTax;
		std::random_device rndD;
		std::mt19937 eng{rndD()};
		std::uniform_real_distribution<float> roleta{0, 1};


		thread_local static std::vector<std::pair <int,int>> twoSwap;

	public:
		RCPMS(int numberJobs_, int numberTools_, std::vector<int>  JobTools_, int numberMachine_, int changeTax_, std::vector<int> JobTax_);
		~RCPMS();
		void setNeighborType(int nType);
		void setMoveForce(int mF);

		solution construction();
		solution neighbor(solution sol);
		solution neighbor2opt(solution sol);
		solution neighbor2Swap(solution sol);
		solution neighborRandIns(solution sol);
		int evaluate(solution* sol);
		int evaluate2(solution* sol);
		void setCut(solution* sol);
		void setCutUp(solution* sol);
		void setCutDown(solution* sol);
		int KTNS(solution sol);
		int getNumberJobs();
		int getNumberTools();
		std::vector<int>* getJobTools();
};

#endif 
