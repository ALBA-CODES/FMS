#ifndef __PT_H__
#define __PT_H__


#include <vector>
#include <deque> 
#include <thread>         
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <math.h>
#include <algorithm>
#include <bits/stdc++.h>
#include "SSP.h"
#include "Consumer.h"
#include "NodeMCMC.h"
#include "NodeSwap.h"
#include "ExecTime.h"


class PT{
	private:
		// PT varibles
		float tempMin_; // init temp
		float tempMax_; // end temp
		int tempL_; // temp size
		int MKL_; // MC length
		int PTL_; // number of swaps
		int tempUpdate; // number of swaps to update de temp
		int numberJobs;
		int numberTools; 
		int capacityMagazine;
		int sum = 0;
		std::string fileName;
		std::vector<std::vector<bool>>  toolJob;
		std::vector<std::vector<int>>  JobTools;
		std::map<std::pair<int,int>, int> difJob;
		std::deque<double> allTemps;
	public:
		PT(float tempMin, float tempMax, int tempL, int MKL, int PTL, int tempUp, int tempD); // constructor
		~PT();
		void start(int nt, int mf, int ut);
		void stop();
		void calcBest();
		void printResult();
		void printBestSol();
		void generateFiles();
		void loadInstance(std::string filename);
		std::deque<double> tempPG(float tempMin, float tempMax, int tempL); 
		std::deque<double> tempExp(float tempMin, float tempMax, int tempL); 
		std::deque<double> tempLinear(float tempMin, float tempMax, int tempL); 
		std::deque<double> tempILinear(float tempMin, float tempMax, int tempL); 
};

#endif
