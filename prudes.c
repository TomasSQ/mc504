#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "prudes.h"

void* prudes_f(void *param) {
	while (prudes < MAX_PRUDES) {
		printf("prudes\n");
		prudes++;

		if (status == NEUTRAL) {
			status = PRUDES_RULE;
			sem_post(&mutex);
		} else if (status == HEATHENS_RULE) {
			if (prudes > heathens) {
				status = TRANSITION_TO_PRUDES;
				sem_wait(&heathensTurn);
			}

			sem_post(&mutex);
			sem_wait(&prudesQueue);
		} else if (status == TRANSITION_TO_PRUDES) {
			sem_post(&mutex);
			sem_wait(&prudesQueue);
		} else
			sem_post(&mutex);

	  /* cross the field */
	 
		sem_wait(&mutex);
		prudes--;

		if (prudes == 0) {
			if (status == TRANSITION_TO_HEATHENS)
				sem_post(&heathensTurn);
			if (heathens) {
		    	sem_post_many(&heathensQueue, prudes);
		    	status = PRUDES_RULE;
			} else
		    	status = NEUTRAL;
		}

		if (status == PRUDES_RULE && heathens > prudes) {
			status = TRANSITION_TO_HEATHENS;
			sem_wait(&prudesTurn);
		}

		sem_post(&mutex);
		sleep(1);
	}

	return NULL;
}
