#include "utils.hpp"
//algorithm
#define MEM(i,j) mem[((i)*(p.w+1))+(j)]
int exact(Problem p){ 
	uint n=p.prices.size();
	uint* mem=(uint*)calloc(sizeof(uint),(n+1)*(p.w+1));
	for(uint i=0;i<=n;i++){
		for(uint j=0;j<=p.w;j++){
			if((i==0) ||(j==0))
				MEM(i,j) = 0;
			else if (p.weights[i-1] <= j)
				MEM(i,j) = std::max(p.prices[i-1] + MEM(i-1,j-p.weights[i-1]),  MEM(i-1,j)); 
			else
				MEM(i,j) = MEM(i-1,j); 
		}
	}
	return MEM(n,p.w);
}
//---------------------------------------------------------------------------------------------------
int main(int argc, char const *argv[]){
	COMMON_ARGS
	MAKE_TEST(
		w=exact(p);
	)
	return 0;
}