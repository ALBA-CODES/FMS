/*
 * PT.cpp
 * 
 * Copyright 2021 André Luis <André Luis@DESKTOP-HDL2CBS>
 *  
 * 
 */

#include "../include/PT.h"
 
// inicialização do PT. valores da temp. tamanho da MCMC e o Número de propostas de troca entre temp
PT::PT(float tempMin, float tempMax, int tempL, int MKL, int PTL, int tempUp, int tempD, int stopC)
	: tempMin_(tempMin)
	, tempMax_(tempMax)
	, tempL_(tempL)
	, MKL_(MKL)
	, PTL_(PTL)
	, tempUpdate(tempUp)
{
	if(stopC == 0) stopC_ = 0.1 * PTL;
	else stopC_ = stopC;	
	
	switch (tempD){
		case 1:{
			allTemps = tempLinear(tempMin,tempMax,tempL);
		}
		break;
		case 2:{
			allTemps = tempILinear(tempMin,tempMax,tempL);
		}
		break;
		case 3:{
			allTemps = tempExp(tempMin,tempMax,tempL);
		}
		break;
		case 4:{
			allTemps = tempPG(tempMin,tempMax,tempL);
		}
		break;
		default:
			cout << "Invalid initial temperature distribution! \n";
		break;	
	}

} 

PT::~PT(){

}

void PT::start(int nt, int mf, int ut, int thN){	
	ExecTime et;
	Consumer* consumer = new Consumer(thN, stopC_, &et);
	atomic<int> PTLEnd = PTL_;

	//variaveis
	Node* nMCMC;
	Node* nSwap;
	Node* nUpTemp;	
	Node* nMCMCAux;
	Node* nSwapAux;
	Node* nUpTempAux;
		    
    // Cria o primeiro nó MCMC
    IPMTC* prob_= new IPMTC(numberJobs,numberTools,capacityMagazine,toolJob,JobTools, difJob, sum, numberMachine, changeTax, JobTax);
    prob_->setNeighborType(nt);	
    prob_->setMoveForce(mf);
    thN-=2;	
	nMCMC = new NodeMCMC(MKL_,&PTLEnd,allTemps.front(), prob_, consumer, (thN>1));
	((NodeMCMC*)nMCMC)->setFirstTemp(); // check First temp 
	consumer->setMaxEnd();
	allTemps.pop_front();
	nMCMCAux = nMCMC; 
		
    // Cria o segundo nó MCMC
    prob_= new IPMTC(numberJobs,numberTools,capacityMagazine,toolJob,JobTools, difJob, sum, numberMachine, changeTax, JobTax);	
    prob_->setNeighborType(nt);	
    prob_->setMoveForce(mf);
    thN-=2;	
	nMCMC = new NodeMCMC(MKL_,&PTLEnd,allTemps.front(), prob_, consumer, (thN>1));
	consumer->setMaxEnd();
	allTemps.pop_front();
	
	// Cria o primeiro nó de troca
	if(!ut) nSwap = new NodeSwap(nMCMC,nMCMCAux, consumer, &PTLEnd);
	else{ 
		nUpTemp = new NodeTempUp(nMCMC,nMCMCAux, NULL, consumer,ut, tempL_);
		nSwap = new NodeSwap(nMCMC,nMCMCAux,nUpTemp, consumer, &PTLEnd, tempUpdate);	
	}
	
	consumer->setMaxEnd();


	// arestas chegando no swap	
	nSwap->addEdge(nMCMCAux,nSwap);
	nSwap->addEdge(nMCMC,nSwap);
	
	// arestas saindo do swap
	nSwap->addEdge(nSwap, nMCMCAux);
	nSwap->addEdge(nSwap, nMCMC);
	
	// atualiza as arestas
	nMCMCAux->addEdge(nMCMCAux,nSwap);
	nMCMC->addEdge(nMCMC,nSwap);
	nMCMCAux->addEdge(nSwap,nMCMCAux);
	nMCMC->addEdge(nSwap,nMCMC);

	
	// comeca a execucao	
	consumer->execAsync(nMCMCAux);

	// recupera o nó a direita
	nMCMCAux = nMCMC;
	nSwapAux = nSwap;
	nUpTempAux = nUpTemp;
	
	// Cria os nós restantes	
	while(!allTemps.empty()){

		prob_= new IPMTC(numberJobs,numberTools,capacityMagazine,toolJob,JobTools, difJob, sum, numberMachine, changeTax, JobTax);
		prob_->setNeighborType(nt);
		prob_->setMoveForce(mf);
		thN-=2;	
		nMCMC = new NodeMCMC(MKL_,&PTLEnd, allTemps.front(),prob_, consumer, (thN>1));
		consumer->setMaxEnd();
		allTemps.pop_front();

		// cria o nó de troca
		if(!ut) nSwap = new NodeSwap(nMCMC,nMCMCAux,consumer,&PTLEnd);
		else {
			nUpTemp = new NodeTempUp(nMCMC,nMCMCAux, nUpTempAux, consumer,ut, tempL_);
			((NodeTempUp*)nUpTempAux)->setUpTempUp(nUpTemp);
			nSwap = new NodeSwap(nMCMC,nMCMCAux,nUpTemp,consumer, &PTLEnd, tempUpdate);
		}
		
		consumer->setMaxEnd();
		
		// Atualiza as arestas que chegam
		nSwap->addEdge(nMCMCAux,nSwap);
		nSwap->addEdge(nMCMC,nSwap);
		nSwap->addEdge(nSwapAux,nSwap);
		nMCMCAux->addEdge(nSwap, nMCMCAux);
		nMCMC->addEdge(nSwap,nMCMC);		

		//atualiza as arestas saem 
		nMCMCAux->addEdge(nMCMCAux,nSwap);
		nMCMC->addEdge(nMCMC,nSwap);			
		nSwapAux->addEdge(nSwapAux,nSwap);
		nSwap->addEdge(nSwap, nMCMCAux);
		nSwap->addEdge(nSwap, nMCMC);

		//start exec
		consumer->execAsync(nMCMCAux);
		
		//update varibles
		nMCMCAux = nMCMC;
		nSwapAux = nSwap;
		nUpTempAux = nUpTemp;
	}

	//check last temp
	((NodeMCMC*)nMCMCAux)->setLastTemp();
	if(ut)((NodeTempUp*)nUpTempAux)->setUpTempUp(NULL);
	//start exec
	consumer->execAsync(nMCMCAux);


consumer->finished();
solution best = consumer->getBestSol();

if(!((NodeMCMC*)nMCMCAux)->onFase2()){
        cout<<"Fase 2: disable \n";
         
	float lowerBound = (float)(totalJobTax + (best.evalSol * changeTax))/(float)numberMachine;

	int job = 0;
	for(int np = 1; np < numberMachine; ++np){
	
		int makespamJ = 0;
		int makespamT = 0;
		std::vector<int> s;
	
		do{
			makespamJ += JobTax[best.sol[job]];
			s.push_back(best.sol[job]);
			makespamT = changeTax * prob_->KTNS5(s);
			++job;
		}while((makespamJ + makespamT) < lowerBound);
	
		best.cutStart[np] = job;
		best.cutEnd[np -1] = job;
	}

	best.evalSol = INT_MAX; 
	setCut(&best, prob_);
}


et.getTime(tempMin_,tempMax_,tempL_,MKL_,PTL_,fileName,best,numberMachine);
cout<<"Best sol: "<<best.evalSol<<" PTL best sol: "<<*consumer->getIndexPT()<<"\n";
}

void PT::setCut(solution* sol, IPMTC* prob){
	
	solution s = *sol;
	solution best = *sol;
		
	int max = 0;
	int cm = -1;
	
	while(cm != (numberMachine-1)){
		max = 0;
		cm = -1;
		
		for(int i = 0; i < numberMachine; ++i){
			std::vector<int> sv = std::vector<int>(s.sol.begin() + s.cutStart[i], s.sol.begin() + s.cutEnd[i]);		
			int tax = prob->GPCAIPMTC(sv);				
			if(tax > max){
				max = tax;
				cm = i;
			}
		}
		
		if(best.evalSol > max){
			s.evalSol = max;
			best = s;
		}
		
		if((cm+1) < numberMachine){
			--s.cutEnd[cm]; 
			--s.cutStart[cm+1];
		}
		
	}
	
		s = best;
		
		while(cm != 0){
		max = 0;
		cm = -1;
		
		for(int i = 0; i < numberMachine; ++i){
			std::vector<int> sv = std::vector<int>(s.sol.begin() + s.cutStart[i], s.sol.begin() + s.cutEnd[i]);		
			int tax = prob->GPCAIPMTC(sv);				
			if(tax > max){
				max = tax;
				cm = i;
			}
		}
	
		if(best.evalSol > max){
			s.evalSol = max;
			best = s;
		}
		
		if((cm-1) >= 0){
			++s.cutEnd[cm-1]; 
			++s.cutStart[cm];
		}
		
	}

	*sol = best;
}


std::deque<double> PT::tempPG(float tempMin, float tempMax, int tempL){
	// variaveis
	std::deque<double> temps(tempL);
	double b = tempMax/tempMin;
	double ex = 1.0/((double)(tempL - 1));	
	double r =  std::pow(b,ex); 
	// Calculando as temp
	for(unsigned int ind = 0; ind < temps.size();++ind){
		temps[ind] = tempMin * std::pow(r,ind);	
	}

	return temps;
}
std::deque<double> PT::tempExp(float tempMin, float tempMax, int tempL){
	// variaveis
	std::deque<double>temps(tempL);
	double tempMaxExp = log(tempMax);
	double tempMinExp = log(tempMin);

	// Calculando as temp
	for(unsigned int ind = 0; ind < temps.size();++ind){
		double currentTemp = tempMinExp + ((tempMaxExp - tempMinExp) * ((double)ind/(double)(tempL - 1)));
		temps[ind] = exp(currentTemp);
	}
	return temps;
} 
std::deque<double> PT::tempLinear(float tempMin, float tempMax, int tempL){
	// variaveis
	std::deque<double> temps(tempL);
	
	// Calculando as temp
	for(unsigned int ind = 0; ind < temps.size();++ind){
		double currentTemp = tempMin + ((tempMax - tempMin) * ((double)ind/(double)(tempL - 1)));
		temps[ind] = currentTemp;
	}
	
	return temps;
}

std::deque<double> PT::tempILinear(float tempMin, float tempMax, int tempL){
	// variavel com as temps
	std::deque<double> temps(tempL);
	double bm = 1.0/tempMax;
	double b1 = 1.0/tempMin;
	
	// Calculando as temp
	for(unsigned int ind = 0; ind < temps.size();++ind){
		// Calcular a temp
		double bi = bm + ((b1-bm) * ((double)ind/(double)(tempL - 1)));
		temps[ind] = 1.0 / bi;
	}
	return temps;
}


void PT::stop(){
	
}

void PT::printResult(){
	
}

void PT::printBestSol(){

}

void PT::generateFiles(){

}

void PT::loadInstance(std::string filename){
	
	fileName = filename;
	std::string line; 
    std::ifstream ifs;
	int i=0;
	int j=0;
	ifs.open(filename);		

	if ( ifs.is_open()){
		
		getline(ifs,line);
		std::stringstream s(line);
		getline (s,line, ' ');
		numberMachine = std::stoi(line);
		getline (s,line, ' ');		
		numberJobs = std::stoi(line);
		getline(s,line, ' ');		
		numberTools = std::stoi(line);
		getline(s,line, ' ');		
		capacityMagazine = std::stoi(line);
		
		getline(ifs,line);
		changeTax = std::stoi(line);
		getline(ifs,line);		
		std::stringstream ss(line);

		toolJob.assign(numberTools, std::vector<bool>(numberJobs,false));
		JobTools.resize(numberJobs);
		
		while ( getline(ss,line, ' ')){
			totalJobTax += std::stoi(line);
			JobTax.push_back(std::stoi(line));
		}
					
		while(getline(ifs,line)){
			
			std::stringstream sss(line);
			while ( getline (sss ,line, ' ')){
				toolJob[i][j] = std::stoi(line);
				if(toolJob[i][j]){ 
					JobTools[j].push_back(i);
					++sum;
					}
			j++;
			}
		j=0;
		i++;		
		}
	ifs.close();
	
	}else{
		std::cout << "Could not open! \n";
	}	
}

void PT::calcBest(){
		
	for(int i = 0; i< (numberJobs-1); ++i){
		for(int ii = i+1; ii < numberJobs; ++ii){
			int par = 0;

			for(int j = 0;j<numberTools;++j){
				if((!toolJob[j][i] && toolJob[j][ii]) || (toolJob[j][i] && !toolJob[j][ii])) ++par;
			}
			
		difJob.insert(std::make_pair(std::make_pair(i,ii),par));
		difJob.insert(std::make_pair(std::make_pair(ii,i),par));		
		}
	}
}
