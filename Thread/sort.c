#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>

#define NR_CPU 2
#define NR_TOTAL 400
#define NR_CHILD (NR_TOTAL/NR_CPU)

typedef struct Param{
	// int num[NR_TOTAL];
	int start;
	int end;
}Param;

typedef struct Result{
	int num[NR_TOTAL];
}Result;

int num[NR_TOTAL];
void *child_sort(void *arg){
	Param* param;
	Result* result;
	param = (Param*)arg;
	int i = 0, j = i,s = 0;
	// printf("start is %d, end is %d\n", param->start, param->end);
	for(i = param->start;i < param->end;i++){
		s = i;
		for(j = i; j < param->end; j++){
			if(num[s] > num[j]){
				s = j;
			}
		}
		int temp = 0;
		temp = num[i];
		num[i] = num[s];
		num[s] = temp;
	}
	return NULL;
}

int main(){
	pthread_t thread[NR_CPU];
	Param params[NR_CPU], *param;
	int i = 0;
	srand(time(NULL));
	for(i = 0; i < NR_TOTAL; i++){
		num[i] = (((int)rand()*1000)%(NR_TOTAL+ 4));
		/*
		if((i + 1)%10 == 0)
			printf("\n");
		printf("%d ", num[i]);
		*/
	}
	for(i = 0; i < NR_CPU - 1; i++){
		param = &params[i];
		param->start = i * NR_CHILD;
		param->end = (i + 1) * NR_CHILD;

		/* sort at child thread*/
		pthread_create(&thread[i], NULL, child_sort, param);
	}
	param = &params[i];
	param->start = i * NR_CHILD;
	param->end = (i + 1) *NR_CHILD;

	/* sort at parent thread */
	child_sort((void*)param);

	for(i = 0; i < NR_CPU - 1 ; i++){
		pthread_join(thread[i], NULL);
	}
	/*
	printf("first block\n");
	for(i = 0; i < NR_CHILD ; i++){
		if((i + 1) % 10 == 0)
			printf("\n");
		printf("%d ", num[i]);
	}
	
	printf("begin to conbime\n");
	*/
	/* combine two array */
	int result[NR_TOTAL];
	int ptr = 0, j = 0;
	i = 0; j = NR_CHILD;
	for(ptr = 0;ptr < NR_TOTAL; ptr ++){
		if(i < NR_CHILD && j < 2 * NR_CHILD){
			if (num[i]< num[j]){
				result[ptr] = num[i++];
			}
			else{
				result[ptr] = num[j++];
			}
		}
		else if (i < NR_CHILD){
			result[ptr] = num[i++];
		}
		else{
			result[ptr] = num[j++];
		}
	}

	for(i = 0; i < NR_TOTAL; i++){
		if((i + 1) % 10 == 0){
			printf("\n");
		}
		printf("%d ", result[i]);
	}
	printf("\n");
	return 0;
}
