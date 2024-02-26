#ifndef __PROBLEM_H__
#define __PROBLEM_H__


#include <vector>
#include <climits>



struct solution{ 
  std::vector<int> sol; 
  std::vector<int> cutEnd; 
  std::vector<int> cutStart; 

  int evalSol = INT_MAX; 
  int cm = -1;
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
		virtual void evaluate(solution* sol)=0;
};

#endif
