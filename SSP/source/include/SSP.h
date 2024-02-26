#ifndef __SSP_H__
#define __SSP_H__

#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <utility>
#include <forward_list>
#include <list>
#include <set>
#include <map>
#include <omp.h>
#include <random>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <thread>
#include <atomic>
#include "Problem.h"


class SSP: public Problem{
	private:
		int numberJobs;
		int numberTools;
		int nType_;
		int moveF;
		int capacityMagazine;
		int meio;
		int sum;
		std::vector<std::vector<bool>>  toolJob;
		std::vector<std::vector<int>>  JobTools;
		std::map<std::pair<int,int>, int> difJob;
		thread_local static std::vector<std::pair <int,int>> twoSwap;

	public:
		SSP(int numberJobs_, int numberTools_, int capacityMagazine_,std::vector<std::vector<bool>>  toolJob_, std::vector<std::vector<int>>  JobTools_, std::map<std::pair<int,int>, int> difJob_, int sum_);
		~SSP();
		void setNeighborType(int nType);
		void setMoveForce(int mF);
		std::pair<int,double> KTNS(solution sol);
		std::pair<int,double> KTNS5(solution sol);
		void GPCAD(solution sol, int* last_fullD, int* pipes_countD, std::vector<int>* last_seenD, std::vector<std::vector<int>>* MD);
		int GPCAE(solution sol, int* last_fullD, int* pipes_countD, std::vector<int>* last_seenD, std::vector<std::vector<int>>* MD, std::atomic_bool* end);
		int GPCA(solution sol);


		solution construction();
		solution neighbor(solution sol);
		solution neighbor2opt(solution sol);
		solution neighbor2Swap(solution sol);
		solution neighborRandIns(solution sol);
		float evaluate(solution sol);
		int getNumberJobs();
		int getNumberTools();
		int getCapacityMagazine();
		int calcBestJoin(solution* neigh);
		std::vector<std::vector<bool>>*  getToolJob();
		std::vector<std::vector<int>>* getJobTools();
		int fixSolution(std::vector<bool>* L, std::vector<bool>* R, int j);
};

#endif 
