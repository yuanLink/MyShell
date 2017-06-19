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

int buffer_is_empty(){
	// printf("(empty) now in = %d, out = %d\n",in,out);
	return in == out;
}

int buffer_is_full(){
	// printf("(full) now in = %d, out = %d\n",in,out);
	return (in + 1)%CAPACITY == out;
}

char get_item(){

	int item;
	item = buffer1[out];
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
		pthread_cond_signal(&wait_empty_buffer);
		pthread_mutex_unlock(&mutex);			
	}	 
	return NULL;
}

void calculator(){
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
	pthread_t consumer_t;
	
	in = out = 0;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&wait_empty_buffer, NULL);
	pthread_cond_init(&wait_full_buffer ,NULL);

	pthread_create(&consumer_t, NULL, consumer, NULL);

	producer(NULL);
	pthread_join(consumer_t, NULL);
	return 0;
}
