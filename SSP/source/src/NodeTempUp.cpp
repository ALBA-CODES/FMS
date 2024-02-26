/*
 * NodeTempUp.cpp
 * 
 * Copyright 2021 André Luis <André Luis@DESKTOP-HDL2CBS>
 *  
 */
#include "../include/NodeTempUp.h"

NodeTempUp::NodeTempUp(Node* left_, Node* right_,Node* downTempUp_, Consumer *pool_, int ut_, int tL)
:left(left_)
,right(right_)
,downTempUp(downTempUp_)
,pool(pool_)
,m(tL)
,ut(ut_)
{}

NodeTempUp::~NodeTempUp(){
}

void NodeTempUp::run(){
    switch (ut){
		case 1:{
			tempUp23();
		}
		break;
		case 2:{
			tempUpEnergy();
		}
		break;
		case 3:{
			tempUpFB();
		}
		break;
		default:
			cout << "Tipo de atualização de temp inválida! \n";
		break;
	}
}

void NodeTempUp::tempUp23(){

    if(acceptRate < 0.2) {

		double k = (energyR - energyL)/((NodeMCMC*)right)->getTemp();
		double newT = (energyL - energyR) / (log(0.23) - k);
		if(newT > 0.0)((NodeMCMC*)left)->setTemp(newT);
	}

}

void NodeTempUp::tempUpEnergy(){
	deltaE = energyL - energyR;
	
	if(downTempUp){
		double k1 = ((NodeTempUp*)downTempUp)->getDeltaE();
		double tMenos = ((NodeTempUp*)downTempUp)->getDownTemp();
		double eq = ((k1/tMenos) + (deltaE/((NodeMCMC*)left)->getTemp()));
		double newT = ((deltaE+k1)/eq);
		if(newT > 0.0)((NodeMCMC*)right)->setTemp(newT);
	}
}

void NodeTempUp::tempUpFB(){
	
	if(!downTempUp){
		double density = this->getProbDens();
		double dfdt = ((fl - fr)/(((NodeMCMC*)left)->getTemp() - ((NodeMCMC*)right)->getTemp()));//(fr - fl)
		if((dfdt != 0) && (density != 0) && (!isnan(density))){
		c = 1.0/density;
		double eq = pow((1.0/(double)(m*c)),2)/abs(dfdt);
		double newT = eq + ((NodeMCMC*)right)->getTemp();
		if((newT > ((NodeMCMC*)right)->getTemp()) && ((newT < ((NodeTempUp*)upTempUp)->getLeftTemp())))((NodeMCMC*)left)->setTemp(newT);
		}
	}else if(upTempUp){
		c = ((NodeTempUp*)downTempUp)->getC();
		double dfdt = ((fl - fr)/(((NodeMCMC*)left)->getTemp() - ((NodeMCMC*)right)->getTemp()));
		if((dfdt != 0) && (c != 0)){
		double eq = abs(pow((1.0/(double)(m*c)),2)/dfdt);
		double newT = eq + ((NodeMCMC*)right)->getTemp();
		if((newT > ((NodeMCMC*)right)->getTemp()) && ((newT < ((NodeTempUp*)upTempUp)->getLeftTemp())))((NodeMCMC*)left)->setTemp(newT);	
		}
	}
	
}

bool NodeTempUp::ready(){
	
	for(vector<std::pair<Node*,bool>>::iterator it = edgeto.begin(); it != edgeto.end(); it++){
		if(!it->second) return false;
	}
	
	return true;
}

bool NodeTempUp::notify(){
	
	for(vector<std::pair<Node*,bool>>::iterator it = edgeFrom.begin(); it != edgeFrom.end(); it++){
		Node* to = it->first;
		if(!to->observer(this))return false;	
	}
	
	return true;
}

bool NodeTempUp::observer(Node* from){
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

	if((ready) && (!finish())){ 
			pool->execAsync(this);

	}
	
	return status;
}


void NodeTempUp::start(){
	pool->execAsync(this);
}

void NodeTempUp::setacceptRate(double aR){
	acceptRate = aR;
}

void NodeTempUp::setEnergy(double l, double r){
 energyL = l;
 energyR = r;
}

double NodeTempUp::getDeltaE(){
 return deltaE;
}

double NodeTempUp::getDownTemp(){
	return ((NodeMCMC*)right)->getTemp(); 
}

void NodeTempUp::setUpTempUp(Node* utu){
	upTempUp = utu;
}

double NodeTempUp::getProbDens(){
	double den;
	double atual;
	
	// Get flow
	fr = ((NodeMCMC*)right)->getFlow();
	fl = ((NodeMCMC*)left)->getFlow();
	//(fl > fr)
	if((fr == 0) || (fl==0)) return 0;

	((NodeMCMC*)right)->resetFlow();
	double dfdt = abs(((fl - fr)/(((NodeMCMC*)left)->getTemp() - ((NodeMCMC*)right)->getTemp())));

	den = ((NodeMCMC*)left)->getTemp() - ((NodeMCMC*)right)->getTemp(); 
	//abs(fr - fl)
	atual = sqrt(dfdt/den)*den;
	if(upTempUp)atual += ((NodeTempUp*)upTempUp)->getProbDens();
	return atual;
}

double NodeTempUp::getC(){
	return c;
}
double NodeTempUp::getLeftTemp(){
	return ((NodeMCMC*)left)->getTemp();
}




