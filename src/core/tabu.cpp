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
typedef struct TabuTable{
	TabuTable(uint n,uint t_size):table(n,true),indexes(t_size,-1),iterator(0){}
	bool check_entry(uint i){
		return table[i];
	}
	void update_entry(uint i){
		int index=indexes[iterator];
		if(index!=-1){
			table[index]=true;
		}
		table[i]=false;
		indexes[iterator]=i;
		iterator=(iterator+1)%indexes.size();
	}
	std::vector<bool> table;
	std::vector<int> indexes;
	uint iterator;
}TabuTable;
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
Solution get_greedy_solution(Problem p,std::vector<ProblemEntry> problem_entry){
	uint n=problem_entry.size();
	Solution ans(n,0,0);
	for(uint i=0;i<n;i++){
		ProblemEntry entry=problem_entry[i];
		if(ans.w+entry.weight<=p.w){
			ans.p+=entry.price;
			ans.w+=entry.weight;
			ans.vec[i]=true;
		}
	}
	return ans;
}
Solution local_search(Problem p,std::vector<ProblemEntry> ps,Solution s,TabuTable& table,Solution best_sol){
	uint n=ps.size();
	Solution ans=s;
	int bit_chosed=-1;
	for(uint i=0;i<n;i++){
		ProblemEntry entry=ps[i];
		if(!s.vec[i]){ //bit not set
			if(s.w+entry.weight > p.w) //inviable
				continue;
			if(!table.check_entry(i) && s.p+entry.price <= best_sol.p) //is tabu and is not the best ans
				continue;
			
			if(bit_chosed==-1 || s.p+entry.price > ans.p){
				bit_chosed=i;
				ans=s;
				ans.p+=entry.price;
				ans.w+=entry.weight;
			}
		}
		else{ // bit is set
			if(s.w-entry.weight > p.w) //inviable
				continue;	
			if(!table.check_entry(i) && s.p-entry.price <= best_sol.p) //is tabu and is not the best ans
				continue;
			if(bit_chosed==-1 || s.p-entry.price > ans.p){
				bit_chosed=i;
				ans=s;
				ans.p-=entry.price;
				ans.w-=entry.weight;
			}
		}
	}

	if(bit_chosed != -1){
		table.update_entry(bit_chosed);
		ans.vec[bit_chosed]=ans.vec[bit_chosed]^true;
	}
	return ans;
}

int tabu(Problem p,uint t_size,uint btmax){
	uint n=p.prices.size();
	std::vector<ProblemEntry> greedy_vec=get_greedy(p);
	Solution c=get_greedy_solution(p,greedy_vec);
	Solution best_sol=c;
	TabuTable table(n,t_size);
	for(uint iter=1;iter<=btmax;iter++){
		Solution lc=local_search(p,greedy_vec,c,table,best_sol);
		if(c.p==lc.p)
			break;
		if(lc.p>best_sol.p){
			iter=0;
			best_sol=c;
		}
		c=lc;
	}
	return best_sol.p;
	
}
//---------------------------------------------------------------------------------------------------
int main(int argc, char const *argv[]){
	std::srand(SEED);
	if(argc!=4){
		fprintf(stderr, "[USAGE] %s [input.in] [t_size] [btmax]\n",argv[0]);
		return 1;
	}
	Problem p=read_instance(argv[1]);
	uint t_size=atoi(argv[2]);
	uint btmax=atoi(argv[3]);
	MAKE_TEST(
		w=tabu(p,t_size,btmax);
	)
	return 0;
}