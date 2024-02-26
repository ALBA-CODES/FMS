/*
 * NodeMCMC.cpp
 * 
 * Copyright 2021 André Luis <André Luis@DESKTOP-HDL2CBS>
 *  
 * 
 */
#include "../include/NodeMCMC.h"

NodeMCMC::NodeMCMC(int MCL_, atomic<int>* PTL_, double temp_, IPMTC* prob_,Consumer* pool_, bool SP)
:MCL(MCL_)
,temp(temp_)
,pool(pool_)
{
	execMax = PTL_;
	prob = prob_;
	MCMCType = true;
	 	
	indexPT = pool_->getIndexPT();
	stopC_ = pool_->getStopC();
	startTime =  pool_->getStart();
	
	sol = prob->construction();
	bestSol = sol;
//	exec = SP;

	if(SP){
		exec = [this]() { runParallel();};
		th2 = true;
		pool->execAsync(this);
	}else{
		exec = [this]() { runSerial();};	
	}

}

NodeMCMC::~NodeMCMC(){
}

void NodeMCMC::run(){
 
exec();

}

void NodeMCMC::threadRun(){
	cout<<"Parallel MCMC.\n";
	th2 = false;
	while(true){
		if(!fase){			
			prob->GPCAD(neigh, &last_fullD, &pipes_countD, &last_seenD, &MD);
			
			fase = true;			
			final = true;
		}
		if(stop) break;
	}
	cout<<"End parallel MCMC.\n";
}

void NodeMCMC::runParallel(){
	
	if(th2){
		th2 =false; 
		threadRun();
		return;
	}
	
	// calc MCMC	
	for (int i = 0; i < MCL; i++){
		// gerar o vizinho					
		neigh = prob->neighbor(sol);
		neighAux = neigh; 
		fase = false;
		
		if(!fase2){ 
			neigh.evalSol = prob->GPCAE(neighAux, &last_fullD, &pipes_countD, &last_seenD, &MD, &final);
		}else {
			prob->setCut(&neigh);
			stop = true;
		}
		
		setBestSol(neigh);
				
		//calc do delta
		double delta = neigh.evalSol - sol.evalSol;
					
		// Menor ou igual a 0, aceitação sempre de 100%
		if (delta <= 0.0){
			// 
			sol = neigh;
			++accept;
		}else{
			//termina o calc da pocentagem de aceitacao
			double param, probab;
			param = -(delta/temp);
			probab = exp(param);
					
			//Aceita a troca
			if(dis(gen) <= probab){
				sol = neigh;
				++accept;
			} //fim if troca prob					
		} //fim if/else 				
	} //fim for
	
	if(theEnd()) {
		stop = true;
		pool->theEnd(bestSol);
		endN = true;
	}
	
	reset();	
	notify();

}

void NodeMCMC::runSerial(){
	
	for (int i = 0; i < MCL; i++){
		// gerar o vizinho					
				
		neigh = prob->neighbor(sol);
		if(!fase2)neigh.evalSol = prob->GPCA(neigh.sol);
		else prob->setCut(&neigh);
							
		setBestSol(neigh);
		
		
		//calc do delta
		double delta = neigh.evalSol - sol.evalSol;
					
		// Menor ou igual a 0, aceitação sempre de 100%
		if (delta <= 0.0){
			// 
			sol = neigh;
			++accept;
		}else{
			//termina o calc da pocentagem de aceitacao
			double param, probab;
			param = -(delta/temp);
			probab = exp(param);
					
			//Aceita a troca
			if(dis(gen) <= probab){
				sol = neigh;
				++accept;
			} //fim if troca prob					
		} //fim if/else 				
	} //fim for

	if(theEnd()){ 
		pool->theEnd(bestSol);
		endN = true;
	}

	reset();	
	notify();
}

bool NodeMCMC::ready(){
	
	for(vector<std::pair<Node*,bool>>::iterator it = edgeto.begin(); it != edgeto.end(); it++){
		if(!it->second) return false;
	}
	
	return true;
}

bool NodeMCMC::notify(){
	
	for(vector<std::pair<Node*,bool>>::iterator it = edgeFrom.begin(); it != edgeFrom.end(); it++){
		Node* to = it->first;
		if(!to->observer(this))return false; 	
	}
	
	return true;
}

bool NodeMCMC::observer(Node* from){
	bool status = false;
	bool ready = true;
	
	mtxNode.lock();
	for(vector<std::pair<Node*,bool>>::iterator it = edgeto.begin(); it != edgeto.end(); it++){
		
		if(it->first == from){
			it->second = true;
			status = true;
		}
		
		if(!it->second) ready = false;
	}
	mtxNode.unlock();
	
	if((ready) && (!finish() || !endN)){
		pool->execAsync(this);
		
	}
	return status;
}

double NodeMCMC::getTemp(){
	return temp;
}

int NodeMCMC::getEnergia(){
	return sol.evalSol;
}

solution NodeMCMC::getSol(){
	return sol;
}
void NodeMCMC::setSol(solution sol_){
	sol = sol_;
	setBestSol(sol);
}

bool NodeMCMC::onFase2(){
	return fase2;
}


bool NodeMCMC::setBestSol(solution sol_){
	if (sol_.evalSol<bestSol.evalSol) {
		bestSol=sol_;	
		pool->setBestSol(sol_,execAtual,fase2);
		return true;
	}
	return false;
}

void NodeMCMC::start(){
	pool->execAsync(this);
}

void NodeMCMC::setTemp(double t){
	temp = t;
}

double NodeMCMC::getAccept(){
	double rate = ((double)accept/(double)(execAtual*MCL));
	accept = 0;
	return rate;
}

void NodeMCMC::setLastTemp(){
	lastTemp = true;
}
void NodeMCMC::setFirstTemp(){
	firstTemp = true;
}

void NodeMCMC::trySetLabels(){
	if(firstTemp){
		sol.Nup = true;
		sol.Ndown = false;
	} else if(lastTemp){
		sol.Nup = false;
		sol.Ndown = true;
	}
}

void NodeMCMC::updateFlow(){
	if(sol.Nup) ++nUp;
	else if(sol.Ndown) ++nDown;
}

double NodeMCMC::getFlow(){	
	
	if((nUp+nDown) == 0) return 0;
	double f = ((double)nUp/(double)(nUp+nDown));
	return f; 
}

void NodeMCMC::resetFlow(){	
		
	nUp = 0;
	nDown = 0;
}

