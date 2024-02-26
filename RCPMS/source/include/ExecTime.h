#ifndef __EXECTIME_H__
#define __EXECTIME_H__

#include <chrono>
#include <iostream>
#include <string>
#include <fstream>
#include "RCPMS.h" 

class ExecTime{
	private:
		std::chrono::high_resolution_clock::time_point start;

	public:
		ExecTime();
		void getTime(float tempMin, float tempMax, int tempL, int MKL, int interNumber,std::string filename_, solution best, int nm);
		std::chrono::high_resolution_clock::time_point getStart();
		~ExecTime();
};

#endif
