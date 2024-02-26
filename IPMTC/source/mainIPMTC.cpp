/*
 * main.cpp
 * 
 * Copyright 2021 André Luis <André Luis@DESKTOP-HDL2CBS> 
 * 
 */
#include <cstdlib>
#include "./include/ExecTime.h"
#include "./include/PT.h"


using namespace std;

int main(int argc, char* argv[])
{
	//varibles
	float tempIni = 0.01;
	float tempfim = 2.0;
	int tempN = 10;
	int MCL = 0;
	int PTL = 2;	
	int tempUp = 50;
	int tempD = 1;
	int mType = 3;
	int uType = 0;
	int mForce = 1;	
	int stopC = 0;	
	int thN = thread::hardware_concurrency();
	
	vector<std::string> arguments(argv + 1, argv + argc);	
	// file name
	std::string fn = arguments[0];
	

	for(unsigned int i=1; i<arguments.size(); i+=2)
	{
		            
        if(arguments[i]== "--TEMP_FIM")
            tempfim = std::stof(arguments[i+1]);
        else if(arguments[i]== "--TEMP_INIT")
            tempIni = std::stof(arguments[i+1]);
        else if(arguments[i]== "--N_REPLICAS")
            tempN = std::stoi(arguments[i+1]);
        else if(arguments[i]== "--MCL")
            MCL = std::stoi(arguments[i+1]);
        else if(arguments[i]== "--PTL")
            PTL = std::stoi(arguments[i+1]);
        else if(arguments[i]== "--TEMP_DIST")
            tempD = std::stoi(arguments[i+1]);
        else if(arguments[i]== "--MOV_TYPE")
            mType = std::stoi(arguments[i+1]);
        else if(arguments[i]== "--MOV_FORCE")
            mForce = std::stoi(arguments[i+1]);
        else if(arguments[i]== "--TYPE_UPDATE")
            uType = std::stoi(arguments[i+1]);
        else if(arguments[i]== "--TEMP_UPDATE")
            tempUp = std::stoi(arguments[i+1]);
        else if(arguments[i]== "--THREAD_USED")
            thN = std::stoi(arguments[i+1]);
        else if(arguments[i]== "--STOP_PTL")
            stopC = std::stoi(arguments[i+1]);
    }
		
	// inicialização do PT. valores da temp. tamanho da MCMC e o Número de propostas de troca entre temp
	PT *algo;
	algo = new PT(tempIni,tempfim,tempN,MCL,PTL,tempUp, tempD, stopC);
	algo->loadInstance(fn);
	std::cout<<"Start PT. \n";
	algo->start(mType, mForce, uType, thN);

	return 0;
}

