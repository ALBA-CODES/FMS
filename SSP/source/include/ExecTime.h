#ifndef __EXECTIME_H__
#define __EXECTIME_H__

#include <chrono>
#include <iostream>
#include <string>
#include <fstream> 

class ExecTime{
	private:
		std::chrono::high_resolution_clock::time_point start;

	public:
		ExecTime();
		void getTime(float tempMin, float tempMax, int tempL, int MKL, int interNumber, int best,std::string filename_);
		~ExecTime();
};

#endif
