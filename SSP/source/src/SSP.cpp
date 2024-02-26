/*
 * Problem.cpp
 * 
 * Copyright 2021 André Luis <André Luis@DESKTOP-HDL2CBS> 
 * 
 */

#include "../include/SSP.h"

thread_local std::vector<std::pair <int,int>> SSP::twoSwap;

SSP::SSP(int numberJobs_, int numberTools_, int capacityMagazine_,std::vector<std::vector<bool>>  toolJob_, std::vector<std::vector<int>>  JobTools_, std::map<std::pair<int,int>, int> difJob_, int sum_)
	:numberJobs(numberJobs_)
	,numberTools(numberTools_)
	,moveF(1)
	,capacityMagazine(capacityMagazine_)
	,meio(numberJobs_/2)
	,sum(sum_)
	,toolJob(toolJob_)
	,JobTools(JobTools_)
	,difJob(difJob_)
	{}

SSP::~SSP(){
	
}

void SSP::setNeighborType(int nType){
	nType_= nType;
}

void SSP::setMoveForce(int nF){
	moveF = nF;
}

std::pair<int,double> SSP::KTNS(solution sol){
	
	std::vector<std::vector<bool>>  t(numberTools, std::vector<bool>(numberJobs));
	std::set<std::pair<int, int>> l;	
	int firstInst = 0;
	int somat = 0;
	int somaj = 0;
	int evalSol = 0; 
	double block0 = 0.0;
	
		
	while(firstInst != capacityMagazine){	
		if (!t[somat][0]){
			t[somat][0] = toolJob[somat][sol.sol[somaj]];
				if(t[somat][0]) firstInst++;	
		}
		++somat;
		if(somat == numberTools){
			somat = 0;
			++somaj;
		}
	}

	
	
	for(int j=1; j< numberJobs; j++){
		int need = 0;
		auto ini = std::chrono::high_resolution_clock::now();
		for(int i=0; i < numberTools; i++){
						
			//if (j > 0 ){
				
				if((!t[i][j-1]) and (toolJob[i][sol.sol[j]])){
					t[i][j] = toolJob[i][sol.sol[j]];
					need++;					
				}else if((t[i][j-1]) and (!toolJob[i][sol.sol[j]])){
					int tc = j;
					bool exit = true;
					while((tc < numberJobs) and (exit)){
						if(toolJob[i][sol.sol[tc]]){
							l.insert(std::pair<int, int>(tc,i));
							exit = false;
						}else if(tc==(numberJobs-1)){
								l.insert(std::pair<int, int>((tc+1),i));
						}					
						tc++;
					} 										
				}else{
					t[i][j] = toolJob[i][sol.sol[j]];
				} 
		//	}				  
		} //fim for todas as ferramentas
		
		auto duracao = std::chrono::high_resolution_clock::now() - ini;
			int cont = 0;
		    for (auto it = l.crbegin(); it != l.crend(); ++it) {
				if(cont < need){
					t[(*it).second][j] = false;
					if((*it).first < numberJobs){					
						block0 += sqrt(((*it).first-j));
					}
				}else{
					t[(*it).second][j] = true;
				}
				cont++;
			}
			evalSol+= need;
			need = 0; 
			l.clear();
			
			auto duracao1 = std::chrono::high_resolution_clock::now() - ini;
	}
	
	
	return std::make_pair (evalSol,block0);
}



std::pair<int,double> SSP::KTNS5(solution sol){
	
	int evalSol = 0; 
	double block0 = 0.0;
	int jL;
	std::vector<bool> magazineL(numberTools, true);

	for(jL= 0; jL < numberJobs; ++jL){
		
		std::vector<bool> magazineCL(numberTools);		
		int left = jL;
		int cmL = 0;
		while((cmL < capacityMagazine) && (left < numberJobs)){
			for (auto it=JobTools[sol.sol[left]].begin(); ((it!=JobTools[sol.sol[left]].end()) && (cmL < capacityMagazine)); ++it){
				if((magazineL[*it]) && (!magazineCL[*it])){
					magazineCL[*it] = true;
					++cmL;
				}else if((jL == left) && (!magazineCL[*it])){
					magazineCL[*it] = true;
					//++trocaCL;
					++evalSol;
					++cmL;
				}
			}
			++left;
		}
		
		for(int t=0; ((t < numberTools) && (cmL < capacityMagazine)); t++){
			if((magazineL[t]) && (!magazineCL[t])){
				magazineCL[t] = true;
				++cmL;			
			}
		}
		
		magazineL = magazineCL;
	}

	return std::make_pair (evalSol,block0);
}


void SSP::GPCAD(solution sol, int* last_fullD, int* pipes_countD, std::vector<int>* last_seenD, std::vector<std::vector<int>>* MD){
			int pipes_count = 0;
			int last_full = (numberJobs-1);
			std::vector<int> last_seen(numberTools);
			std::vector<std::vector<int>> M;
	

			//Completa o last_seen
			for(int i = 0; i < numberTools; ++i){
				if(toolJob[i][sol.sol[(numberJobs-1)]]) last_seen[i] = (numberJobs-1);
				else last_seen[i] = numberJobs;
			}
			
			M.push_back(JobTools[sol.sol[(numberJobs-1)]]);
	
	
			for(int e = (numberJobs-2); e >= meio; --e){
		
				M.push_back(JobTools[sol.sol[e]]);
				
				for (auto t = JobTools[sol.sol[e]].begin(); t != JobTools[sol.sol[e]].end(); ++t){
			
					if(last_full >= last_seen[*t]){
				
						++pipes_count;
								
						for(int i = (last_seen[*t]-1); i > e; --i){
					
							M[((numberJobs-1) - i)].push_back(*t);					
							if((int)M[((numberJobs-1) - i)].size() == capacityMagazine) last_full = i;
						}
					}
					last_seen[*t] = e; 	
				}
		
				if((int)M[((numberJobs-1) - e)].size() == capacityMagazine) last_full = e;
			}

			
			*last_fullD = last_full;
			*last_seenD = last_seen;
			*MD = M;
			*pipes_countD = pipes_count;
}

int SSP::GPCAE(solution sol, int* last_fullD, int* pipes_countD, std::vector<int>* last_seenD, std::vector<std::vector<int>>* MD, std::atomic_bool* end){

	int pipes_count = 0;
	int last_full = 0;
	std::vector<int> last_seen(numberTools);
	std::vector<std::vector<int>> M;
	

	//Completa o last_seen
	for(int i = 0; i < numberTools; ++i){
		if(toolJob[i][sol.sol[0]]) last_seen[i] = 0;
		else last_seen[i] = -1;
	}
	M.push_back(JobTools[sol.sol[0]]);
	
	
	for(int e = 1; e <= meio; ++e){
		
		M.push_back(JobTools[sol.sol[e]]);
				
		for (auto t = JobTools[sol.sol[e]].begin(); t != JobTools[sol.sol[e]].end(); ++t){
			
			if(last_full <= last_seen[*t]){
				
				++pipes_count;
				
				
				for(int i = (last_seen[*t]+1); i < e; ++i){
					
					M[i].push_back(*t);					
					if((int)M[i].size() == capacityMagazine) last_full = i;
				}
			}
			last_seen[*t] = e; 	
		}
		
		if((int)M[e].size() == capacityMagazine) last_full = e;
	}

	
	while(!(*end)){}
	(*end) = false;


	for(int e = (meio+1); e <= (*last_fullD); ++e){
		
		M.push_back((*MD)[((numberJobs-1) - e)]);
				
		for (auto t = JobTools[sol.sol[e]].begin(); t != JobTools[sol.sol[e]].end(); ++t){
			
			if((last_full <= last_seen[*t]) && (last_seen[*t] != meio)){
				
				++pipes_count;
								
				for(int i = (last_seen[*t]+1); i < e; ++i){
					
					M[i].push_back(*t);					
					if((int)M[i].size() == capacityMagazine) last_full = i;
				}
			last_seen[*t] = -1; 	
			}
		}
		
		if((int)M[e].size() == capacityMagazine) last_full = e;
	}


 int result = (sum - capacityMagazine - (pipes_count+(*pipes_countD)));
 if(result < 0) result = 0;  
 
 return result;
  
}

int SSP::GPCA(solution sol){
	
	int pipes_count = 0;
	int last_full = 0;
	std::vector<int> last_seen(numberTools);
	std::vector<std::vector<int>> M;
	

	//Completa o last_seen
	for(int i = 0; i < numberTools; ++i){
		if(toolJob[i][sol.sol[0]]) last_seen[i] = 0;
		else last_seen[i] = -1;
	}
	M.push_back(JobTools[sol.sol[0]]);
	
	
	for(int e = 1; e < numberJobs; ++e){
		
		M.push_back(JobTools[sol.sol[e]]);
				
		for (auto t = JobTools[sol.sol[e]].begin(); t != JobTools[sol.sol[e]].end(); ++t){
			
			if(last_full <= last_seen[*t]){
				
				++pipes_count;
				
				
				for(int i = (last_seen[*t]+1); i < e; ++i){
					
					M[i].push_back(*t);					
					if((int)M[i].size() == capacityMagazine) last_full = i;
				}
			}
			last_seen[*t] = e; 	
		}
		
		if((int)M[e].size() == capacityMagazine) last_full = e;

	}


 int result = (sum - capacityMagazine - pipes_count);
 if(result < 0) result = 0;  

 return result; 
}

solution SSP::construction(){
	solution ss;
    std::random_device rnd_device;
    std::mt19937 mersenne_engine {rnd_device()}; 
    	
	for(int i=0; i< numberJobs; i++){
		ss.sol.push_back(i);
	}
	
	std::shuffle(begin(ss.sol), end(ss.sol), mersenne_engine);
	
	std::pair<int,double> kt = KTNS5(ss); 
	ss.evalSol = kt.first;
	ss.block0Sol = kt.second;
	ss.Nup = false;
	ss.Ndown = false;
	 
	return ss;

}

solution SSP::neighbor(solution sol){
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

solution SSP::neighborRandIns(solution sol){
	solution s;
	s.sol = sol.sol;
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

solution SSP::neighbor2opt(solution sol){
	solution s;
	s.sol = sol.sol;
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

solution SSP::neighbor2Swap(solution sol){
	solution s;
	s.sol = sol.sol;
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

float SSP::evaluate(solution sol){
	return (float)sol.evalSol;
}

int SSP::getNumberJobs(){
	return numberJobs;
}

int SSP::getNumberTools(){
	return numberTools;
}

int SSP::getCapacityMagazine(){
	return capacityMagazine;
}

std::vector<std::vector<bool>>* SSP::getToolJob(){
	return &toolJob;
}

std::vector<std::vector<int>>* SSP::getJobTools(){
	return &JobTools;
}

int SSP::fixSolution(std::vector<bool>* L, std::vector<bool>* R, int j){

	int free = capacityMagazine - JobTools[j].size();
	std::vector<bool> magazine(numberTools, false); 	
	int add = 0;
	
	for(int c = 0;c < numberTools; ++c){
		if(!(*L)[c] && toolJob[c][j]) ++add;
					
		if((*L)[c] && (*R)[c] && (!toolJob[c][j]) && (free > 0)){ 
			magazine[c] = true;
			--free;
		}
					
		if(toolJob[c][j]) magazine[c] = true;
	}
				
	for(int c = 0;c < numberTools; ++c){
		if(!magazine[c] && (*R)[c]) ++add;
	}
	return add;
}

int SSP::calcBestJoin(solution* neigh){
	int max = 0;
	int meioS = (numberJobs/2)-1;
	int range = numberJobs * 0.1;
	int meioR = (numberJobs/2)-1;

	for(int jp = (meioS - range); jp < (meioS + range); jp++){
		if(difJob[{neigh->sol[jp],neigh->sol[jp+1]}] > max){ 
			max = difJob[{neigh->sol[jp],neigh->sol[jp+1]}]; 
			meioR = jp+1;
		}
	}
	return meioR;
}


