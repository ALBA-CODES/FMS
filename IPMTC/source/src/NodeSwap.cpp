/*
 * NodeSwap.cpp
 * 
 * Copyright 2021 André Luis <André Luis@DESKTOP-HDL2CBS>
 *  
 */
#include "../include/NodeSwap.h"

NodeSwap::NodeSwap(Node* left_, Node* right_, Consumer *pool_, atomic<int>* PTL_)
:left(left_)
,right(right_)
,pool(pool_)
{
	execMax = PTL_;
	
	indexPT = pool_->getIndexPT();
	stopC_ = pool_->getStopC();

	replicaEstL = new ReplicaEst();
	replicaEstR = new ReplicaEst();
}

NodeSwap::NodeSwap(Node* left_, Node* right_,Node* tempUp_ , Consumer* pool_, atomic<int>* PTL_, int UPL_)
:left(left_)
,right(right_)
,tempUp(tempUp_)
,pool(pool_)
,upTime(UPL_)
{
	execMax = PTL_;

	indexPT = pool_->getIndexPT();
	stopC_ = pool_->getStopC();
	startTime =  pool_->getStart();

	replicaEstL = new ReplicaEst();
	replicaEstR = new ReplicaEst();
}

NodeSwap::~NodeSwap(){
}

void NodeSwap::run(){
	// Verifica se chegou ao final da execução
	if(theEnd()){
		pool->theEnd();
		endN = true;
	}
	//cout<<"Node Swap!! \n";

	double temp1 = ((NodeMCMC*)left)->getTemp();
	double temp2 = ((NodeMCMC*)right)->getTemp();
	double energia1 = ((NodeMCMC*)left)->getEnergia();
	double energia2 = ((NodeMCMC*)right)->getEnergia();
	replicaEstL->Push(energia1);
	replicaEstR->Push(energia2);
	
	// calculo do expoente da exp
	double deltaBeta = ((1.0/temp1) - (1.0/temp2)) * (energia1-energia2);
	//Quando delta Belta maior e igual a 0 sempre troca
	if(deltaBeta >= 0){									
		swap();							
		++accept;
	}else{
		//calc da probabilidade de troca
		double probab;
		probab = exp(deltaBeta);
		//Troca aceita
		if(dis(gen) <= probab){
			swap();	
			++accept;
		} 
	} // fim if/else
			
	// reseta as dependências
	reset();
	
	if((execAtual%upTime) == 0){
		((NodeTempUp*)tempUp)->setacceptRate((double)accept/(double)execAtual);
		((NodeTempUp*)tempUp)->setEnergy(replicaEstL->Mean(), replicaEstR->Mean());
		tempUp->run();
		accept = 0;
		replicaEstL->Clear();
		replicaEstR->Clear();		
	}
		 
	// Notifica os nós a frente
	notify();
	
}

bool NodeSwap::ready(){
	
	for(vector<std::pair<Node*,bool>>::iterator it = edgeto.begin(); it != edgeto.end(); it++){
		if(!it->second) return false;
	}
	
	return true;
}

bool NodeSwap::notify(){
	
	for(vector<std::pair<Node*,bool>>::iterator it = edgeFrom.begin(); it != edgeFrom.end(); it++){
		Node* to = it->first;
		if(!to->observer(this))return false;	
	}
	
	return true;
}

bool NodeSwap::observer(Node* from){
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

void NodeSwap::swap(){
	solution aux = ((NodeMCMC*)left)->getSol();
	((NodeMCMC*)left)->setSol(((NodeMCMC*)right)->getSol());
	((NodeMCMC*)right)->setSol(aux); 
	
	// tentar setar os labels up en down
	((NodeMCMC*)left)->trySetLabels();
	((NodeMCMC*)right)->trySetLabels();
	
	// update flow
	((NodeMCMC*)left)->updateFlow();
	((NodeMCMC*)right)->updateFlow();
	
}
void NodeSwap::start(){
	pool->execAsync(this);
}
