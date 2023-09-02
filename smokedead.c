/*In the Cigarette-Smokers Problem, Consider a system with three smoker processes and one agent process. Each smoker continuously rolls a cigarette and then smokes it. But to roll and smoke a cigarette, the smoker needs three ingredients: tobacco, paper and matches. One of the smoker processes has paper, another has tobacco and the third has matches. The agent has an infinite supplyof all three materials. The agent places two of the ingredients on the table. The smoker who has the remaining ingredient then makes and smokes a cigarette, signaling the agent on completion. The agent then puts out another two of the three ingredients and the cycle repeats. Write a program to synchronize the agent and the smokers.
Code:*/
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

const int PAPER = 1;
const int TOBACCO = 2;
const int MATCHES = 3;

typedef struct Smoker
{   char smokerName[30];
    int item;
} smoker;

typedef struct Agent{   
    char agentName[30];   
    int item1;
    int item2;
} agent;

void printAgents(agent* ag);
void printSmoker(agent* ag, smoker* sm);
char* getIngredientName(int choice);

int main()
{   
    srand(time(NULL));
    agent ag;   
    smoker sm[3] = {{"Paper", 1}, {"Tobacco", 2}, {"Matches", 3}};
    int choice = 0;
    strcpy(ag.agentName, "Agent");   
    ag.item1 = 0;
    ag.item2 = 0;
    
    printf("The first five process:\n");
    do{
        for(int i = 0; i < 5; i++){
            printAgents(&ag);
            usleep(1000000);
            printSmoker(&ag, sm);
            usleep(2000000);
        }       
        printf("\nEnter an integer for next five process (-1 to exit): ");
        scanf("%d", &choice);   
    }while(choice != -1);   
    
    return 0;
}

void printAgents(agent* ag)
{   
    ag->item1 = 1 + rand() % 3;
    
    while(1){
        ag->item2 = 1 + rand() % 3;
        if(ag->item1 != ag->item2)           
        break;   
    }   
    
    printf("\nAgent provides ingredients - %s, %s\n", getIngredientName(ag->item1), getIngredientName(ag->item2));
}
void printSmoker(agent* ag, smoker* sm)
{   
    int i = 0;   
    
    while(sm[i].item != 0){       
        if((sm[i].item != ag->item1) && (sm[i].item != ag->item2)){           
            printf("Smoker with %s is smoking his cigarette\n", sm[i].smokerName);
            ag->item1 = 0;
            ag->item2 = 0;
            break;
        }
        i++;
    }
}

char* getIngredientName(int choice)
{   
    if(choice == PAPER)
        return "Paper";
    else if(choice == TOBACCO)
        return "Tobacco";
    else if(choice == MATCHES)
        return "Matches";
    else 
        return "None";
}