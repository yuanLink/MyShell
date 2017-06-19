#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
// #include <sys/type.h>
#include <semaphore.h>

#define N 20

sem_t sem[N];
pthread_mutex_t mutex;
int num;

typedef struct argv{
	int id;
	int next_id;
}my_argv;

void init_my_sem(){
	int i = 0;
	for(i = 0; i < N; i++){
		sem_init(&sem[i], 1, 0);
	}
	sem_init(&sem[0], 1, 1);
}

void* calcNumber(void* arg){

	my_argv* my_id = (my_argv*)arg;
	while(1){
		sem_wait(&sem[my_id->id]);
		pthread_mutex_lock(&mutex);
		num += 1;
		printf("thread[%d]-->thread[%d]:num is %d\n",my_id->id ,my_id->next_id, num);
		sleep(1);
		sem_post(&sem[my_id->next_id]);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}
int main(){
	pthread_t thread[N];
	my_argv ids[N];

	init_my_sem();
	pthread_mutex_init(&mutex,NULL);
	
	int i = 0;
	for(i = 0; i < N; i++){
		ids[i].id = i;
		ids[i].next_id = (i + 1)%N;
		pthread_create(&thread[i], NULL, calcNumber, &ids[i]);
	}
	for(i = 0; i < N; i++){
		pthread_join(thread[i], NULL);
	}
	return 0;
}
