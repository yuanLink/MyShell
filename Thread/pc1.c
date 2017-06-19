#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

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
	return (out + 1)%CAPACITY == in;
}

/*****************
 *  buffer_is_calculate
 * when out + 1 == calc ,means that the output will output last one 
 * means that this one we can not output
 *
 ****************/
int buffer_is_calculate(){
	// printf("(calc) now in = %d, cal = %d\n",in,cal);
	return (out + 1)%CAPACITY == cal;
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

pthread_mutex_t mutex;
pthread_cond_t wait_empty_buffer;
pthread_cond_t wait_full_buffer;
pthread_cond_t wait_for_calc;

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
		pthread_mutex_lock(&mutex);
		// if buffer is full, that we need to wait until the buffer is not full
		while(buffer_is_full())
			pthread_cond_wait(&wait_full_buffer, &mutex);
		/*
		// now, the buffer is not full, we need wait the calc
		while(calc_is_doing())
			pthread_cont_wait(&wait_for_calc, &mutex);
		*/
		item = 'a' + i;
		printf("produce item %c\n", item);
		put_item(item);

		// send full to tell that others producer can begin
		pthread_cond_signal(&wait_for_calc);
		pthread_mutex_unlock(&mutex);			
	}	 
	return NULL;
}

void *calculator(void* arg){
	int i;
	for(i = 0; i < ITEM_COUNT; i++){
		pthread_mutex_lock(&mutex);
		// printf("calc is waiting...\n");
		while(buffer_is_calculate())
			pthread_cond_wait(&wait_for_calc, &mutex);
		calc_item();

		pthread_cond_signal(&wait_empty_buffer);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

void *consumer(void* arg){
	int i;
	char item;

	for(i = 0;i < ITEM_COUNT; i++){
		pthread_mutex_lock(&mutex);

		// printf("(consumer)now i is %d\n",i);
		while(buffer_is_empty())
			pthread_cond_wait(&wait_empty_buffer, &mutex);
		item = get_item();
		printf("(consumer)consumer item is %c\n",item);
		
		// send empty to tell that others consumer can begin
		pthread_cond_signal(&wait_full_buffer);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main(){
	pthread_t consumer_t, calculator_t;
	
	in = out = 0;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&wait_empty_buffer, NULL);
	pthread_cond_init(&wait_full_buffer ,NULL);
	pthread_cond_init(&wait_for_calc ,NULL);

	pthread_create(&consumer_t, NULL, consumer, NULL);
	pthread_create(&calculator_t, NULL, calculator, NULL);

	producer(NULL);
	pthread_join(consumer_t, NULL);
	pthread_join(calculator_t, NULL);
	return 0;
}
