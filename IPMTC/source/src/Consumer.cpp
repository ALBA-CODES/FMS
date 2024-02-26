/*
 * Consumer.cpp
 * 
 * Copyright 2021 André Luis <André Luis@DESKTOP-HDL2CBS>
 * 
 * 
 */

#include "../include/Consumer.h"

Consumer::Consumer(unsigned PTL, int stopC, ExecTime* time){
	
	threadCount = PTL;
	stopC_ = stopC;
	timeEnd = time;
	
	for (unsigned n = 0; n != threadCount; ++n) {
		buff.emplace_back();
	}
	
	for (unsigned n = 0; n != threadCount; ++n) {
		threads.emplace_back([&, n]{ run(n); });
	}
}

void Consumer::run(unsigned i){
	
	Node* c;
	while(finish){
	for(unsigned n = 0; n != threadCount; ++n){
		 if((c = buff[(i + n) % threadCount].pop())){
			 c->run();
			break;
		 }
	}
    }
}

void Consumer::finished(){
// End the thread	
	while(!(maxEnd <= end)) finish = true;
	
	finish = false;
	
//	for(auto& b:buff) b.done();	
	for(auto& t:threads) t.join();
}

bool Consumer::theEnd(){
	end++;
	return (maxEnd <= end);	
}

void Consumer::setBestSol(solution b, unsigned i, bool fase){
	if((!fase) && (b.evalSol < bestV)){
		bestV = b.evalSol;
		indexPT = i;
	}
}

atomic<int>* Consumer::getIndexPT(){
 return &indexPT;
}

int Consumer::getStopC(){
	return stopC_;
}

bool Consumer::theEnd(solution sol_){
 
	{
		unique_lock<mutex> lock{mtxSol};
		end++;
		if (sol_.evalSol<bestSol.evalSol) bestSol=sol_;
	}
return (maxEnd <= end);	
}

void Consumer::setMaxEnd(int maxEnd_){
	maxEnd = maxEnd_;	
}

void Consumer::setMaxEnd(){
	++maxEnd;	
}

solution Consumer::getBestSol(){
 return bestSol;	
}

int Consumer::getEnd(){
 return end;	
}

void Consumer::execAsync(Node* item){
	auto i = index++;
	buff[i % threadCount].push(item);
}

std::chrono::high_resolution_clock::time_point Consumer::getStart(){
 return timeEnd->getStart();
}
