#include "utils.hpp"
#define SEED 42u
//algorithm
typedef struct ProblemEntry{
	uint price;
	uint weight;
	double cost_benefice;
}ProblemEntry;
typedef struct Solution{
	Solution(){Solution(0,0,0);}
	Solution(uint n,uint total_weight,uint total_price){
		vec.reserve(n);
		vec.resize(n);
		w=total_weight;
		p=total_price;
	}
	std::vector<bool> vec;
	uint w;
	uint p;
} Solution;
std::vector<ProblemEntry> get_greedy(Problem p){
	std::vector<ProblemEntry> items;
	uint n=p.prices.size();
	items.reserve(n);
	items.resize(n);
	for(uint i=0;i<n;i++){
		uint price=p.prices[i];
		uint weight=p.weights[i];
		items[i]={price,weight,(double)price/weight};
	}
	std::sort(items.begin(),items.end(),[] (const ProblemEntry& a, const ProblemEntry& b){
		return a.cost_benefice>b.cost_benefice;
	});
	return items;
}
#define SHUFFLE() std::random_shuffle( window.begin(), window.end())
Solution semi_greedy(Problem p,std::vector<ProblemEntry> ps,uint window_size){
	uint n=ps.size();
	Solution ans(n,0,0);
	window_size=(window_size==0 || window_size>=n)?n:window_size;
	std::vector<uint> window(window_size); 
	// init
	for(uint i=0;i<window_size;i++){
		window[i]=i;
	}
	SHUFFLE();
	ProblemEntry entry;
	uint index;
	uint limitant=window_size;
	while(limitant<=n){
		for(uint j=0;j<window_size;j++){
			index=window[j];
			entry=ps[index];
			if(ans.w+entry.weight<=p.w){
				ans.p+=entry.price;
				ans.w+=entry.weight;
				ans.vec[index]=true;
			}
			if(limitant<=n)
				window[j]=limitant++;
		}
		SHUFFLE();
	}
	return ans;
}
Solution local_search(Problem p,std::vector<ProblemEntry> ps,Solution s){
	uint n=ps.size();
	Solution old_ans=s;
	Solution new_ans;
	while(true){
		new_ans=old_ans;
		for(uint i=0;i<n;i++){
			if(!old_ans.vec[i]){//if bit is not set
				ProblemEntry entry=ps[i];
				if(	old_ans.w+entry.weight<=p.w && //if it is possible to set the bit 
					old_ans.p+entry.price > new_ans.p    //and it's better then current solution
				  ){
					new_ans=old_ans;//reset it
					//update
					new_ans.p+=entry.price;
					new_ans.w+=entry.weight;
					new_ans.vec[i]=true;
				}
			}
		}
		if(new_ans.p==old_ans.p)
			break;
		old_ans=new_ans;
	}
	return new_ans;
}
int grasp(Problem p,uint window_size,uint no_it){
	uint n=p.prices.size();
	std::vector<ProblemEntry> greedy_vec=get_greedy(p);
	Solution c(n,0,0);
	for(uint k=0;k<no_it;k++){
		Solution nc=semi_greedy(p,greedy_vec,window_size);
		nc=local_search(p,greedy_vec,nc);
		if(nc.p>c.p){
			c=nc;
		}
	}
	return c.p;
}
//---------------------------------------------------------------------------------------------------
int main(int argc, char const *argv[]){
	std::srand(SEED);
	if(argc!=4){
		fprintf(stderr, "[USAGE] %s [input.in] [window_size] [no of iterations]\n",argv[0]);
		return 1;
	}
	Problem p=read_instance(argv[1]);
	uint window_size=atoi(argv[2]);
	uint no_it=atoi(argv[3]);
	MAKE_TEST(
		w=grasp(p,window_size,no_it);
	)
	return 0;
}