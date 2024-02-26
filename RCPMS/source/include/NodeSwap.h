#ifndef __NODESWAP_H__
#define __NODESWAP_H__

#include <limits.h>
#include "NodeTempUp.h"
#include "ReplicaEst.h"


class NodeSwap: public Node{
	private:
		Node* left; 
		Node* right;
		Node* tempUp;
		Consumer* pool;
		int accept = 0;
		int upTime = INT_MAX;
		ReplicaEst* replicaEstL; 
		ReplicaEst* replicaEstR; 
		std::default_random_engine gen;
		std::uniform_real_distribution<double> dis;
	public:
		NodeSwap(Node* left_, Node* right_,Consumer* pool_, atomic<int>* PTL_);
		NodeSwap(Node* left_, Node* right_, Node* tempUp_ ,Consumer* pool_, atomic<int>* PTL_, int UPL_);
		~NodeSwap();
		void swap();
		void run();
		void start();
		bool ready();
		bool notify();
		bool observer(Node* from);
};

#endif
