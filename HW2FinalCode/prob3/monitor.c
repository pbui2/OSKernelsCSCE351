#include "monitor.h"
#include "../SharedResources.h"

Monitor mon;
int bMonInited = 0;

Monitor *Monitor_init() {
	bMonInited = 1;
	
	Monitor *B = calloc(1, sizeof(Monitor));	
	B->queue = *Queue_init(BUFFER_SIZE);
	pthread_cond_init(&B->full, NULL);
	pthread_cond_init(&B->empty, NULL);
	pthread_mutex_init(&B->mutex, NULL);
	B->producers = calloc(NUM_THREADS, sizeof(pthread_t));
	B->consumers = calloc(NUM_THREADS, sizeof(pthread_t));
	return B;
}


void mon_insert(char alpha) {
	if (bMonInited == 0) 
		mon = *Monitor_init();
	
	pthread_mutex_lock(&mon.mutex);
		
	int isFull = Queue_IsFull(&mon.queue);
	if(isFull) {
		//Broadcast to threads that are currently suspended and waiting on the full condition they can start
		pthread_cond_broadcast(&mon.full); 
		//Suspend this thread until the threads that just started broadcast the empty condition. 
		pthread_cond_wait(&mon.empty, &mon.mutex);	
	}
		
	Enqueue(&mon.queue, (void*)alpha);
	Queue_Print(&mon.queue);		
	pthread_mutex_unlock(&mon.mutex);	
}
	
char mon_remove(char replacementChar) {
	if (bMonInited==0) 
		printf("MONITOR NOT INITIALIZED");
	
	pthread_mutex_lock(&mon.mutex);
	
	int isEmpty = Queue_IsEmpty(&mon.queue);
	if (isEmpty) {		
		pthread_cond_broadcast(&mon.empty); //Tell the threads that are waiting on empty condition to start
		pthread_cond_wait(&mon.full, &mon.mutex);//Wait for those threads to broadcast the full condition.	
	}

	char value = (char*)Dequeue(&mon.queue);
	pthread_mutex_unlock(&mon.mutex);
	return value;
}