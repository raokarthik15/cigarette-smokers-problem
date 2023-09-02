#include <pthread.h>    // used to work with threads to create and manage threads
#include <semaphore.h>  // includes the header file for working with semaphores, which are used for                                         .                                        // synchronization of threads
#include <stdbool.h>	// to use bool variables in C
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>      // uses the errno variable, which is used to report errors in C programs
#include <unistd.h>     // uses POSIX operating system API functions, such as usleep
#include <time.h>

//Required semaphores
sem_t smoker_waits[3];
sem_t agent;

//making smokers_t as a pointer to a user-defined thread name
typedef void *(*smokers_t)(void *);

/**
 * Smoker 0 has an infinite supply of paper
**/
void* smoker_0(void* arg)
{
	while (true)
	{
		//introducing random delay in nanoseconds
		nanosleep((struct timespec[]){{0, rand() % 200000000}}, NULL);
		printf("Smoker zero waiting to smoke (has paper)\n\n");
		
		//wait for the agent to signal
		sem_wait(&smoker_waits[0]);
		printf("Smoker zero received matches and tobacco. Now making cigarette and smoking.\n");
		//notify agent about completion
		sem_post(&agent);
	}

	return NULL;
}

/**
 * Smoker 1 has an infinite supply of matches
**/
void* smoker_1(void* arg)
{
	while (true)
	{
		nanosleep((struct timespec[]){{0, rand() % 200000000}}, NULL);
		printf("Smoker one waiting to smoke (has matches)\n\n");

		sem_wait(&smoker_waits[1]);
		printf("Smoker one received paper and tobacco. Now making cigarette and smoking.\n");
		sem_post(&agent);
	}

	return NULL;
}

/**
 * Smoker 2 has an infinite supply of tobacco
 */
void* smoker_2(void* arg)
{
	while (true)
	{
		nanosleep((struct timespec[]){{0, rand() % 200000000}}, NULL);
		printf("Smoker two waiting to smoke (has tobacco)\n\n");

		sem_wait(&smoker_waits[2]);
		printf("Smoker two received matches and paper. Now making cigarette and smoking.\n");
		sem_post(&agent);
	}

	return NULL;
}

/**
 * The main thread handles the agent's arbitration of items.
**/
int main(int argc, char* arvg[])
{
	// Seed our random number since we will be using random numbers
	srand(time(NULL));

	// Initialize our agent semaphore
	sem_init(&agent, 0, 1);

	// Setup our smoker threads and function refs
	pthread_t smoker_threads[3];
	smokers_t smoker_functions[3] = { smoker_0, smoker_1, smoker_2 };

	// Create the smoker threads and initialize the semaphores
	for (int i = 0; i < 3; ++i)
	{
		sem_init(&smoker_waits[i], 0, 0);  // Initializes the semaphores for each smoker with an initial value of 0
		//EAGAIN == there is no data available right now, try again later
		if(pthread_create(&smoker_threads[i], NULL, smoker_functions[i], NULL) == EAGAIN)
		{
			perror("Insufficient resources to create thread\n");
			return 0;
		}
	}

	// Let the agent begin distributing his items
	while (true)
	{
		// Lock the agent until notified
		sem_wait(&agent);
		//Simulate some delay before the next distribution
		usleep(1000000);
		// Distribute two items for one of the smokers
		sem_post(&smoker_waits[rand() % 3]);
	}

	// Join all of the smoker threads on exit
	for (int i = 0; i < 3; ++i)
	{
		pthread_join(smoker_threads[i], NULL);
	}
}
