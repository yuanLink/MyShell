#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <errno.h>
#include <semaphore.h>

#define CAPACITY 4
#define ITEM_COUNT 8

char task[] = "abcdefg";
char buffer1[CAPACITY];
char buffer2[CAPACITY];

int in;
int out;
int cal;

/*****************
 *  buffer_is_empty
 * when out == cal ,means that the buffer is the output is calcucating
 * so, nothing can be outpyt
 *
 ****************/
int buffer_is_empty(){
	// printf("(empty) now in = %d, out = %d\n",in,out);
	return cal == out;
}

/*****************
 *  buffer_is_full
 * when out ==in ,means that the buffer is full,can not write next
 *
 ****************/
int buffer_is_full(){
	// printf("(full) now in = %d, out = %d\n",in,out);
	// return (cal + 1)%CAPACITY == in;
	return (in + 1)%CAPACITY == out;
}

/*****************
 *  buffer_is_calculate
 * when out + 1 == calc ,means that the output will output last one 
 * means that this one we can not output
 *
 ****************/
int buffer_is_calculate(){
	// printf("(calc) now in = %d, cal = %d\n",in,cal);
	return (cal + 1)%CAPACITY == in;
}

char get_item(){
	int item;
	item = buffer2[out];
	out = (out + 1)%CAPACITY;
	return item;
}

void change_item(){
	buffer1[out] = buffer1[out] + 32;
}

void put_item(char item){
	buffer1[in] = item;
	in = (in + 1)%CAPACITY;
}

void calc_item(){
	char item = buffer1[cal] - 32;
	buffer2[cal] = item;
	printf("(calc)buffer1 is %c now the item is %c\n",buffer1[cal] ,item);
	cal = (cal + 1)%CAPACITY;
}

sem_t sem_producer, sem_calculator, sem_consumer;
// key_t key_full, key_empty, key_calc;
pthread_mutex_t mutex;

/*************************
 *		producer
 * create char into buffer1
 * 
 *
 *************************/
void* producer(void* arg){
	int i;
	char item;	

	for(i = 0;i < ITEM_COUNT;i++){
		// if buffer is full, that we need to wait until the buffer is not full
		while(buffer_is_full())
			// pthread_cond_wait(&wait_full_buffer, &mutex);
			sem_wait(&sem_producer);

		pthread_mutex_lock(&mutex);
		item = 'a' + i;
		// printf("[+]=============================[+]\n");
		printf("(produce) item %c\n", item);
		put_item(item);

		// send full to tell that others producer can begin
		// pthread_cond_signal(&wait_for_calc);
		sem_post(&sem_calculator);
		// printf("post sem_calculator\n");
		pthread_mutex_unlock(&mutex);	
		// printf("unlock\n");
	}
	in ++ ;
	// printf("(full)[!] finish\n");
	return NULL;
}

void *calculator(void* arg){
	int i;
	for(i = 0; i < ITEM_COUNT; i++){
		//printf("calc is waiting...\n");
		while(buffer_is_calculate())
			// pthread_cond_wait(&wait_for_calc, &mutex);
			sem_wait(&sem_calculator);
		pthread_mutex_lock(&mutex);
		calc_item();

		// pthread_cond_signal(&wait_empty_buffer);
		sem_post(&sem_consumer);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

void *consumer(void* arg){
	int i;
	char item;

	for(i = 0;i < ITEM_COUNT; i++){

		// printf("(consumer)now i is %d\n",i);
		while(buffer_is_empty())
			// pthread_cond_wait(&wait_empty_buffer, &mutex);
			sem_wait(&sem_consumer);
		pthread_mutex_lock(&mutex);
		item = get_item();
		printf("(consumer)consumer item is %c\n",item);
		// printf("[+]=============================[+]\n");
		// send empty to tell that others consumer can begin
		// pthread_cond_signal(&wait_full_buffer);
		sem_post(&sem_producer);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

void my_init_sem(){

	int res;

	/* because first the sem has to write to  buffer ,
	 * so it begin with 1
	 */
	res = sem_init(&sem_producer, 1, 0);
	if(res != 0){
		perror("error in init sem");
		exit(0);
	}
	res = sem_init(&sem_calculator, 1, 0);
	if(res != 0){
		perror("error in init sem");
		exit(0);
	}
	res = sem_init(&sem_consumer, 1, 0);
	if(res != 0){
		perror("error in init sem");
		exit(0);
	}
}
int main(){
	pthread_t consumer_t, calculator_t;
	
	cal = in = out = 0;
	pthread_mutex_init(&mutex, NULL);
	/*
	pthread_cond_init(&wait_empty_buffer, NULL);
	pthread_cond_init(&wait_full_buffer ,NULL);
	pthread_cond_init(&wait_for_calc ,NULL);
	*/
	my_init_sem();

	pthread_create(&consumer_t, NULL, consumer, NULL);
	pthread_create(&calculator_t, NULL, calculator, NULL);

	producer(NULL);
	pthread_join(consumer_t, NULL);
	pthread_join(calculator_t, NULL);
	return 0;
}
