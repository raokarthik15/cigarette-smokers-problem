#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned int TobaccoAndPaper = 0;
unsigned int PaperAndMatches = 0;
unsigned int MatchesAndTobacco = 0;

unsigned int DoneSmoking = 1;

void Wait(unsigned int *semaphore){
    while(*semaphore<=0);
    (*semaphore)--;
}

void Signal(unsigned int *semaphore){
    (*semaphore)++;
}

void Agent()
{
    while(1)
    {    
        Wait(&DoneSmoking);
        
        srand(time(NULL));
        int produce = rand()%3;
        switch (produce)
        {
        case 0:
            Signal(&TobaccoAndPaper);
            break;
        case 1:
            Signal(&PaperAndMatches);
            break;
        case 2:
            Signal(&MatchesAndTobacco);
            break;
        
        default:
            break;
        }
    }
}

void Smoker_A()
{
    while(1)
    {
        Wait(&TobaccoAndPaper);
        
        printf("Smoker A is now smoking");
        usleep(1000000);
        Signal(&DoneSmoking);
    }
}
void Smoker_B()
{
    while(1)
    {
        Wait(&PaperAndMatches);
        
        printf("Smoker B is now smoking");
        usleep(1000000);
        Signal(&DoneSmoking);
    } 
}
void Smoker_C()
{
    while(1)
    {
        Wait(&MatchesAndTobacco);
        
        printf("Smoker C is now smoking");
        usleep(1000000);
        Signal(&DoneSmoking);
    }
}

int main(int argc, char const *argv[])
{
    Smoker_A();
    Smoker_B();
    Smoker_C();
    Agent();
    return 0;
}