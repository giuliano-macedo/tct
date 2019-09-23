#include "utils.hpp"
Problem read_instance(const char* fname){
	Problem ans;
	FILE* f=fopen(fname,"r");
	char buff[1024];
	while(fgets(buff,1024,f)){
		uint id,price,weight,n;
		n=0;
		sscanf(buff,"%u %u %u \n%n",&id,&price,&weight,&n);
		if(n==0){
			sscanf(buff,"%u\n",&ans.w);
		}
		else{
			ans.prices.push_back(price);
			ans.weights.push_back(weight);
		}
	}
	return ans;
}