/*
 * Consumer.cpp
 * 
 * Copyright 2021 André Luis <André Luis@DESKTOP-HDL2CBS>
 * 
 * 
 */

#include "../include/Consumer.h"

Consumer::Consumer(unsigned PTL,  int stopC){
	
	threadCount = PTL;
	stopInteration = stopC;
	
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
	
	for(auto& t:threads) t.join();
}

bool Consumer::theEnd(){
	end++;
	return (maxEnd <= end);	
}

void Consumer::setBestSol(solution b, unsigned i){
	if(b.evalSol < bestV){
		bestV = b.evalSol;
		indexPT = i;
	}
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

atomic<int>* Consumer::getIndexPT(){
 return &indexPT;
}

atomic<int>* Consumer::getMaxDif(){
 return &maxDif;
}

int Consumer::getStopC(){
	return stopInteration;
}

void Consumer::execAsync(Node* item){
	auto i = index++;
	buff[i % threadCount].push(item);
}
