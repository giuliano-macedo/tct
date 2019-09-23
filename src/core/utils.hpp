#ifndef UTILS
#define UTILS
#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <stdlib.h>
#include <chrono> 
namespace chrono=std::chrono;
#define NOW() chrono::high_resolution_clock::now()
typedef unsigned int uint;
typedef struct Problem{
	std::vector<uint> prices;
	std::vector<uint> weights;
	uint w;
} Problem;
Problem read_instance(const char* fname);
std::vector<Problem> read_instances(const char* dname);
void save_results(std::vector<uint> ws,std::vector<uint> ts,const char* fname);
#define COMMON_ARGS \
	if(argc!=2){ \
		fprintf(stderr, "[USAGE] %s [input.in]\n",argv[0]); \
		return 1; \
	} \
	Problem p=read_instance(argv[1]);
#define MAKE_TEST(code) \
	uint w;\
	auto start = NOW();\
	code \
	auto stop = NOW(); \
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start); \
	printf("%u,%li\n",w,duration.count());

#endif