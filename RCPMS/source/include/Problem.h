#ifndef __PROBLEM_H__
#define __PROBLEM_H__


#include <vector>
#include <climits>



struct solution{ 
  std::vector<int> sol; 
  std::vector<int> cutEnd; 
  std::vector<int> cutStart;
  std::vector<std::pair<int,int>> blockPos; 

  int evalSol = INT_MAX; 

  bool Nup,Ndown;
};

class Problem{
	private:
		solution bestSol;
	public:
		Problem();
		~Problem();
		void setBestSol(solution sol);
		virtual solution neighbor(solution sol)=0;
		virtual int evaluate(solution* sol)=0;
};

#endif
