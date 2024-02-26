#ifndef __POOL_H__
#define __POOL_H__

#include <thread>		// std::this_thread::sleep_for
#include <chrono>		// std::chrono::seconds
#include <queue>		//std::queue
#include <deque>
#include <mutex>
#include <atomic>
#include <iostream> 	
#include <optional>
#include "Node.h"

using namespace std;

class Pool{
	private:
		queue<Node*> data;
		std::mutex mtx;
		atomic_int end = 0;
		int maxEnd = 0;
	public:
		Pool();
		Pool(Pool&& obj);
		Pool& operator=(const Pool& obj);
		~Pool();
		size_t size();
		void push(Node* item);
		Node* pop();
};

#endif
