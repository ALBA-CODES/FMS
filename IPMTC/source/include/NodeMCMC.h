#ifndef __NODEMCMC_H__
#define __NODEMCMC_H__

#include <random>
#include <atomic>
#include "Consumer.h"

class NodeMCMC: public Node{
	private:
		int MCL;
		int meio = 0;
		atomic_int nUp = 0;
		atomic_int nDown = 0;
		int currentMCMC = 0;
		int evalValue = 0;
		double temp;
		solution neigh;
		solution neighAux;
		int accept = 0; 
		Consumer* pool;
		bool firstTemp = false;
		bool lastTemp = false;
		atomic_bool fase = true;
		atomic_bool final = false;
		atomic_bool stop = false;
		atomic_bool th2 = false;
		std::function<void(void)> exec;
		std::default_random_engine gen;
		std::uniform_real_distribution<double> dis;
		int last_fullD;/// auxiliary of GPCA PAR
		vector<int> last_seenD;/// auxiliary of GPCA PAR
		vector<vector<int>> MD;/// auxiliary of GPCA PAR
		int pipes_countD = 0;/// auxiliary of GPCA PAR

	public:
		
		NodeMCMC(int MCL_,atomic<int>* PTL_, double temp_, IPMTC* prob_,Consumer* pool_,bool SP);
		~NodeMCMC();
		void run();
		void runParallel();
		void runSerial();
		bool ready();
		bool notify();
		bool observer(Node* from);
		double getTemp();
		int getEnergia();
		double getAccept();
		void start();
		void setSolUp();
		double getFlow();
		void resetFlow();
		void threadRun();
		solution getSol();
		void updateFlow();
		void setLastTemp();
		void setFirstTemp();
		void trySetLabels();
		void setTemp(double t);
		void setSol(solution sol_);
		bool setBestSol(solution sol_);
		bool onFase2();
};
#endif
