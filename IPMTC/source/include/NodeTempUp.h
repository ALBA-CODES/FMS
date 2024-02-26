#ifndef __NODETEMPUP_H__
#define __NODETEMPUP_H__

#include <random>
#include <math.h>
#include "Pool.h"
#include "NodeMCMC.h"

class NodeTempUp: public Node{
	private:
		Node* left; 
		Node* right;
		Node* downTempUp;
		Node* upTempUp;
		Consumer* pool;
		int m = 0;
		int ut = 0;
		double c = 0;
		double fl = 0;
		double fr = 0;
		double deltaE = 0;
		double energyL = 0;
		double energyR = 0;
		double acceptRate = 0;
		std::vector<Node *> allTemps;
	public:
		void run();
		void start();
		bool ready();
		bool notify();
		~NodeTempUp();
		double getC();
		void tempUpFB();
		void tempUp23();
		double getDeltaE();
		void tempUpEnergy();
		double getDownTemp();
		double getProbDens();
		double getLeftTemp();
		bool observer(Node* from);
		void setUpTempUp(Node* utu);
		void setacceptRate(double aR);
		void setEnergy(double l, double r);
		NodeTempUp(Node* left_, Node* right_,Node* downTempUp_, Consumer* pool_, int ut_, int tL);
};

#endif
