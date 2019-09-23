#include "utils.hpp"
//algorithm
typedef struct ProblemEntry{
	uint price;
	uint weight;
	double cost_benefice;
}ProblemEntry;
int greedy(Problem p){ 
	uint n=p.prices.size();
	std::vector<ProblemEntry> items;
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
	int ans=0;
	int w=0;
	for(ProblemEntry entry:items){
		if(w+entry.weight<=p.w){
			ans+=entry.price;
			w+=entry.weight;
		}
	}
	return ans;
}
//---------------------------------------------------------------------------------------------------
int main(int argc, char const *argv[]){
	COMMON_ARGS
	MAKE_TEST(
		w=greedy(p);
	)
	return 0;
}