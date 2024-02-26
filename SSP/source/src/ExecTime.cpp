/*
 * ExecTime.cpp
 * 
 * Copyright 2021 André Luis <André Luis@DESKTOP-HDL2CBS>
 * 
 * 
 */
 
#include "../include/ExecTime.h"

ExecTime::ExecTime()
	: start(std::chrono::high_resolution_clock::now()) 
	{}

void ExecTime::getTime(float tempMin, float tempMax, int tempL, int MKL, int PTL, int best, std::string filename_){	
	auto duration = std::chrono::high_resolution_clock::now() - start;
    std::string filename = filename_;
    std::string base_filename = filename_.substr(filename_.find_last_of("/\\") + 1);
	std::string::size_type const p(base_filename.find_last_of('.'));
	std::string file_without_extension = base_filename.substr(0, p);
    
        
    std::ofstream timeF;
	timeF.open("time_"+file_without_extension+".txt");
	timeF << "Instance filename: " << filename << "\n";
	timeF << "Min temp: " << tempMin << "\n";
	timeF << "Max temp: " << tempMax << "\n";
	timeF << "Best sol: " << best << "\n";
	timeF << "Temp div: " << tempL << "\n";
	timeF << "MCL: " << MKL << "\n";
	timeF << "PTL: " << PTL << "\n";

    timeF << "Elapsed time was " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << " (ms)\n";
	timeF.close();
	
} 
ExecTime::~ExecTime(){
	
}
