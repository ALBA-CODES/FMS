/*
 * RCPMS.cpp
 * 
 * Copyright 2021 André Luis <André Luis@DESKTOP-HDL2CBS>
 * 
 * 
 */

#include "../include/RCPMS.h"

thread_local std::vector<std::pair <int,int>> RCPMS::twoSwap;

RCPMS::RCPMS(int numberJobs_, int numberTools_, std::vector<int>  JobTools_, int numberMachine_, int changeTax_, std::vector<int> JobTax_)
	:numberJobs(numberJobs_)
	,numberTools(numberTools_)
	,numberMachine(numberMachine_)
	,changeTax(changeTax_)
	,moveF(1)
	,meio(numberJobs_/2)
	,JobTools(JobTools_)
	,JobTax(JobTax_)
	{}

RCPMS::~RCPMS(){
	
}

void RCPMS::setNeighborType(int nType){
	nType_= nType;
}

void RCPMS::setMoveForce(int nF){
	moveF = nF;
}

solution RCPMS::construction(){
	
	solution ss;
	std::vector<int> v(numberTools);
	std::random_device rnd_device;
	std::mt19937 mersenne_engine {rnd_device()};
	
	// create vector
	std::iota(v.begin(), v.end(), 0);	
	std::shuffle(v.begin(), v.end(), mersenne_engine);
	

	int posFirst = 0;
	int posEnd = 0;
	
	for (int &t: v) {
		for(int i= 0; i < numberJobs; i++){
			if(JobTools[i] == t){
				ss.sol.push_back(i);
				++posEnd;
			}
		}
		ss.blockPos.push_back( std::make_pair(posFirst,(posEnd-1)));
		posFirst = posEnd;
	}
	

	int taxa = numberJobs/numberMachine;
	ss.cutStart.push_back(0);
	
	for(int np = 1; np < numberMachine; ++np){
		int p = np * taxa;
		ss.cutEnd.push_back(p);
		ss.cutStart.push_back(p);
	}
	ss.cutEnd.push_back(numberJobs);	
	setCut(&ss); 
	
 
	return ss;
}

solution RCPMS::neighbor(solution sol){
	switch (nType_)
	{
		case 1:{ 
			return neighbor2opt(sol); 
		}
		break;
		case 2:{ 
			return neighbor2Swap(sol); 
		}
		break;
		case 3:{ 
			return neighborRandIns(sol); 
		}
		break;
		default:
			return neighbor2opt(sol); 
		break;	
	}
}

solution RCPMS::neighborRandIns(solution sol){
	solution s;
	s.sol = sol.sol;
	s.cutEnd = sol.cutEnd;
	s.cutStart = sol.cutStart;

	int first = 0;
	int last = 0;
	std::random_device rnd_device;
	std::mt19937 mersenne_engine {rnd_device()};
	std::uniform_int_distribution<int> dist {0, (numberJobs-1)};
	
	for(int i = 0 ; i < moveF ; i++){

		do {		
			first = dist(mersenne_engine);
			last = dist(mersenne_engine);		
		} while(first == last);
	
	
		int value = s.sol[first];
		s.sol.erase(s.sol.begin()+first);
	
		if(last != (numberJobs-1)){
			s.sol.insert(s.sol.begin()+last, value);
		}else{
			s.sol.insert(s.sol.end(), value);	
		}
	}
	
	s.Nup = sol.Nup;
	s.Ndown = sol.Ndown;
				
	return s;
}

solution RCPMS::neighbor2opt(solution sol){

	solution s;
	s.sol = sol.sol;
	s.cutEnd = sol.cutEnd;
	s.cutStart = sol.cutStart;

	int first = 0;
	int last = 0;
	std::random_device rnd_device;
	std::mt19937 mersenne_engine {rnd_device()};
	std::uniform_int_distribution<int> dist {0, (numberJobs-1)};
	
	for(int i = 0 ; i < moveF ; i++){
		do {		
			first = dist(mersenne_engine);
			last = dist(mersenne_engine);		
		} while(first == last);
	
		if (first > last)std::swap(first,last);
	
		while (first < last) {
			std::swap(s.sol[first], s.sol[last]);
			first++;
			last--;
		}
	}
		
	s.Nup = sol.Nup;
	s.Ndown = sol.Ndown;
				
	return s;
}

solution RCPMS::neighbor2Swap(solution sol){
	solution s;
	s.sol = sol.sol;
	s.cutEnd = sol.cutEnd;
	s.cutStart = sol.cutStart;

	std::random_device rnd_device;
	std::mt19937 mersenne_engine {rnd_device()};

	
	if (twoSwap.size()==0){
		for(int pos1 = 0; pos1 < numberJobs; pos1++){		
			for(int pos2 = (pos1+1); pos2 < numberJobs; pos2++){		
				twoSwap.push_back(std::make_pair (pos1,pos2));
			}
		}
	}
	


	std::shuffle(begin(twoSwap), end(twoSwap), mersenne_engine);
	
	
	for(int i = 0;i < moveF; i++){	
		std::swap(s.sol[twoSwap[i].first], s.sol[twoSwap[i].second]);
	}
		
	s.Nup = sol.Nup;
	s.Ndown = sol.Ndown;
	
			
	return s;
}

int RCPMS::KTNS(solution sol){

int trocas = 0;

	for(int i = 1; i < numberJobs; ++i){
		if(JobTools[sol.sol[i-1]] != JobTools[sol.sol[i]]) ++trocas;
	}
	
	return trocas;
}

int RCPMS::evaluate(solution* sol){
	
	int max = 0;
	int m = 0;
	int cm = -1;	
	
	std::queue<int> exec;

	std::vector<int> mSpam(numberMachine,0); //makespan da maquina
	std::vector<int> magazine(numberMachine,0);	//magazine atual da maquina
	std::vector<int> p(numberMachine,0);	
	std::vector<int> fSpamStart(numberTools,0);
	std::vector<int> fSpamEnd(numberTools,0);
	std::vector<int> fMachine(numberTools,-1);
	
	
	for(int i = 0; i < numberMachine;++i){
		magazine[i] = JobTools[(*sol).sol[(*sol).cutStart[i]]];
		exec.push(i);
	}

	
	while(!exec.empty()){
		m = exec.front();
		exec.pop();
							
		// verifica se ainda tem tarefas a ser processada
		if(((*sol).cutStart[m]+p[m]) < (*sol).cutEnd[m]){
				
			// verifica se o makespam onde a ferramenta foi utilizada é maior que o makespam da maquina
			if((fSpamStart[JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]]] <= mSpam[m]) && (fSpamEnd[JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]]] > mSpam[m])){
				// o makespam da maquina recebe o makespam da ferramenta
				mSpam[m] = fSpamEnd[JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]]];
			//	std::cout<<"Adionar gap \n";

			}else{				
				fSpamStart[JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]]] = mSpam[m];
			}
			
			// Verifica se há troca
			if((magazine[m] != JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]]) || ((fMachine[JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]]] != m) && (fMachine[JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]]] != -1))){
				mSpam[m] += changeTax;
			//	std::cout<<"Adionar custo troca \n";
			}
				
			mSpam[m] += JobTax[(*sol).sol[(*sol).cutStart[m]+p[m]]];
				
			magazine[m] = JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]];
			fSpamEnd[JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]]] = mSpam[m];
			fMachine[JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]]] = m;
				
		//	std::cout<<"Maquina: "<<m<<" makespam: "<<mSpam[m]<<" Molde: "<<	JobTools[sol.sol[sol.cutStart[m]+p[m]]]<<" tarefa: "<<sol.sol[sol.cutStart[m]+p[m]]<<" Posicao: "<<p[m]<<"\n";
		}
		
		++p[m];
	
		if(exec.empty()){
			int min = std::numeric_limits<int>::max();
			int machine = -1;
		
			for(int i = 0; i < numberMachine;++i){
				if((min >= mSpam[i]) && (((*sol).cutStart[i]+p[i]) < (*sol).cutEnd[i])){
					machine = i;
					min = mSpam[i];
				}
				if(max < mSpam[i]){ 
					max = mSpam[i];
					cm = i;
				} 			
			}
			
			if(machine != -1) exec.push(machine);
		}
	}
	
	
	(*sol).evalSol = max;
	
	return cm;
}


int RCPMS::evaluate2(solution* sol){
	
	int max = 0;
	int m = 0;
	int cm = -1;	
	
	std::queue<int> exec;

	std::vector<int> mSpam(numberMachine,0); //makespan da maquina
	std::vector<int> magazine(numberMachine,0);	//magazine atual da maquina
	std::vector<int> p(numberMachine,0);	
	std::vector<int> fSpamStart(numberTools,0);
	std::vector<int> fSpamEnd(numberTools,0);
	std::vector<int> fMachine(numberTools,-1);
	
	
	for(int i = 0; i < numberMachine;++i){
		magazine[i] = JobTools[(*sol).sol[(*sol).cutStart[i]]];
		exec.push(i);
	}

	
	while(!exec.empty()){
		m = exec.front();
		exec.pop();
							
		// verifica se ainda tem tarefas a ser processada
		if(((*sol).cutStart[m]+p[m]) < (*sol).cutEnd[m]){
				
			// verifica se o makespam onde a ferramenta foi utilizada é maior que o makespam da maquina
			if((fSpamStart[JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]]] <= mSpam[m]) && (fSpamEnd[JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]]] > mSpam[m])){
				// o makespam da maquina recebe o makespam da ferramenta
				mSpam[m] = fSpamEnd[JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]]];
			//	std::cout<<"Adionar gap \n";

			}else{				
				fSpamStart[JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]]] = mSpam[m];
			}
			
			// Verifica se há troca
			if((magazine[m] != JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]]) || ((fMachine[JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]]] != m) && (fMachine[JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]]] != -1))){
				mSpam[m] += changeTax;
			//	std::cout<<"Adionar custo troca \n";
			}
				
			mSpam[m] += JobTax[(*sol).sol[(*sol).cutStart[m]+p[m]]];
				
			magazine[m] = JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]];
			fSpamEnd[JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]]] = mSpam[m];
			fMachine[JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]]] = m;
				
			std::cout<<"Maquina: "<<m<<" makespam: "<<mSpam[m]<<" Molde: "<<	JobTools[(*sol).sol[(*sol).cutStart[m]+p[m]]]<<" tarefa: "<<(*sol).sol[(*sol).cutStart[m]+p[m]]<<" Posicao: "<<p[m]<<"\n";
		}
		
		++p[m];
	
		if(exec.empty()){
			int min = std::numeric_limits<int>::max();
			int machine = -1;
		
			for(int i = 0; i < numberMachine;++i){
				if((min >= mSpam[i]) && (((*sol).cutStart[i]+p[i]) < (*sol).cutEnd[i])){
					machine = i;
					min = mSpam[i];
				}
				if(max < mSpam[i]){ 
					max = mSpam[i];
					cm = i;
				} 			
			}
			
			if(machine != -1) exec.push(machine);
		}
	}
	
	
	(*sol).evalSol = max;
	
	return cm;
}

int RCPMS::getNumberJobs(){
	return numberJobs;
}

int RCPMS::getNumberTools(){
	return numberTools;
}

std::vector<int>* RCPMS::getJobTools(){
	return &JobTools;
}


void RCPMS::setCut(solution* sol){

	solution s = *sol;
	solution best = *sol;
	int cm = -1;

	
	while(cm != (numberMachine-1)){		

		cm = evaluate(&s);				
	
		if(best.evalSol > s.evalSol){
			best = s;
		}
		
		if((cm+1) < numberMachine){
			--s.cutEnd[cm]; 
			--s.cutStart[cm+1];
		}
		
	}

	s = best;
	
	while(cm != 0){
		
		cm = evaluate(&s);				
	
		if(best.evalSol > s.evalSol){
			best = s;
		}
		
		if((cm-1) >= 0){
			++s.cutEnd[cm-1]; 
			++s.cutStart[cm];
		}
		
	}

	*sol = best;
}

void RCPMS::setCutUp(solution* sol){

	solution s = *sol;
	solution best = *sol;
	int cm = -1;
	
	while(cm != (numberMachine-1)){		

		cm = evaluate(&s);				
	
		if(best.evalSol > s.evalSol){
			best = s;
		}
		
		if((cm+1) < numberMachine){
			--s.cutEnd[cm]; 
			--s.cutStart[cm+1];
		}
		
	}

	*sol = best;
}

void RCPMS::setCutDown(solution* sol){

	solution s = *sol;
	solution best = *sol;
	int cm = -1;
	
	while(cm != 0){
		
		cm = evaluate(&s);				
	
		if(best.evalSol > s.evalSol){
			best = s;
		}
		
		if((cm-1) >= 0){
			++s.cutEnd[cm-1]; 
			++s.cutStart[cm];
		}
		
	}

	*sol = best;
}


