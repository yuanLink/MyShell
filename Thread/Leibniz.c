#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NR_TOTAL 4000
#define NR_CPU 2
#define NR_CHILD (NR_TOTAL/NR_CPU)

typedef struct Param{
	int start;
	int end;
}Param;

typedef struct Result{
	double sum;
}Result;


void *child_sum(void *arg){
	Param* param;
	Result* result;
	param = (Param*)arg;
	int i = 0;
	double sum = 0;
	double sign = 1;
	if(param->start%2 == 0) sign = 1;
	else sign = -1;

	for(i = param->start; i<param->end; i++){
		sum = sum + (1.0/(i*2+1))*sign;
		sign *= -1;

	}
	result = (Result*)malloc(sizeof(Result));
	result->sum = sum;
	return result;
}

int main(){

	pthread_t thread[NR_CPU];
	Param params[NR_CPU];
	int i = 0;
	for(i = 0; i< NR_CPU;i++){
		Param *param;
		param = &params[i];
		param->start = i * NR_CHILD;
		param->end = (i+1) * NR_CHILD;
		pthread_create(&thread[i], NULL, child_sum, param);
	}
	double sum = 0;
	for(i = 0; i < NR_CPU; i++){
		Result* result;
		pthread_join(thread[i], (void**)&result);
		sum += result->sum;
		free(result);
	}
	printf("the answer is %.9f\n",sum);
	return 0;
}
