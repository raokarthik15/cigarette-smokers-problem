#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t agentCond;
pthread_cond_t smokerCond[3];

int itemPresent[3] = {0};

void* agent(void* arg);
void* smoker(void* arg);

int main()
{
    pthread_t agentThread;
    pthread_t smokerThreads[3];
    
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&agentCond, NULL);
    
    for (int i = 0; i < 3; i++)
        pthread_cond_init(&smokerCond[i], NULL);
    
    pthread_create(&agentThread, NULL, agent, NULL);
    
    for (int i = 0; i < 3; i++)
        pthread_create(&smokerThreads[i], NULL, smoker, (void*)(intptr_t)(i + 1));
    
    pthread_join(agentThread, NULL);
    
    for (int i = 0; i < 3; i++)
        pthread_join(smokerThreads[i], NULL);
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&agentCond);
    
    for (int i = 0; i < 3; i++)
        pthread_cond_destroy(&smokerCond[i]);
    
    return 0;
}

void* agent(void* arg)
{
    while (1) {
        pthread_mutex_lock(&mutex);
        
        int item1 = rand() % 3;
        int item2 = (item1 + 1 + rand() % 2) % 3;
        
        itemPresent[item1] = 1;
        itemPresent[item2] = 1;
        
        printf("Agent puts tobacco (T), paper (P), and matches (M) on the table.\n");
        
        pthread_cond_broadcast(&agentCond);
        
        pthread_mutex_unlock(&mutex);
        
        // Agent waits until a smoker takes the items
        usleep(1000000);
    }
    
    return NULL;
}

void* smoker(void* arg)
{
    int smokerID = (intptr_t)arg;
    int item1, item2;
    
    switch (smokerID) {
        case 1: // Smoker with tobacco
            item1 = 1; // Paper
            item2 = 2; // Matches
            break;
        case 2: // Smoker with paper
            item1 = 0; // Tobacco
            item2 = 2; // Matches
            break;
        case 3: // Smoker with matches
            item1 = 0; // Tobacco
            item2 = 1; // Paper
            break;
        default:
            return NULL;
    }
    
    while (1) {
        pthread_mutex_lock(&mutex);
        
        while (itemPresent[item1] || itemPresent[item2]) {
            pthread_cond_wait(&smokerCond[smokerID - 1], &mutex);
        }
        
        printf("Smoker %d makes a cigarette and starts smoking.\n", smokerID);
        
        // Reset the items on the table
        itemPresent[item1] = 0;
        itemPresent[item2] = 0;
        
        pthread_cond_signal(&agentCond);
        
        pthread_mutex_unlock(&mutex);
        
        // Smoker smokes for some time
        usleep(2000000);
    }
    
    return NULL;
}