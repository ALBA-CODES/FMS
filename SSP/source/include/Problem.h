#ifndef __PROBLEM_H__
#define __PROBLEM_H__


#include <vector>
#include <climits>



struct solution{ 
  std::vector<int> sol; 
  int evalSol = INT_MAX; 
  double block0Sol;
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
		virtual float evaluate(solution sol)=0;
};

#endif
