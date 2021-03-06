
#include <stdio.h>
#include "../SharedResources.h"

typedef struct Monitor Monitor;

struct Monitor {
	bool bIsInitialized;
	RingBuffer queue;
	pthread_cond_t full;
	pthread_cond_t empty;
	pthread_mutex_t mutex;
	pthread_t *producers;
	pthread_t *consumers;
};

void mon_insert(char alpha);
char mon_remove(char replacementChar);
Monitor *Monitor_init();

