#include <time.h>

#ifndef RND_H
#define RND_H
#include <stdio.h>
#include "utils.h"
#include <stdlib.h>
#endif

#define RND_PRC (unsigned short)rand() % 80 + 20 // For now the best way to generate random personality values


bool isProbable(unsigned short percentile)
{
    if (percentile > rand() % 100)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int randint(int start, int end)
{
    return rand() % (end - start) + start;
}

float mean(float start, float end)
{
    return (start + end) / 2;
}

typedef struct
{
    unsigned int *distribution;
    unsigned int range; // from 0 to range
    unsigned int _len;
} ProbabilityDistribution;

ProbabilityDistribution *createProbabilityDistribution(unsigned int *distribution, int ln)
{
    ProbabilityDistribution *newD = (ProbabilityDistribution *)malloc(sizeof(ProbabilityDistribution));
    newD->distribution = distribution;
    newD->_len = ln;
    newD->range = 0;
    for (int i = 0; i < ln; i++)
    {
        newD->range += distribution[i];
    }
    return newD;
}

unsigned int getRandomOutcome(ProbabilityDistribution *distribution) // gets a random integer outcome based on a probability distribution
{
    int rnd = randint(0, (int)distribution->range-1);
    unsigned int total = 0;
    for (int i = 0; i < distribution->_len; i++)
    {
        int x, y; // borders of distribution
        x = total;
        y = total+distribution->distribution[i];
        total+=distribution->distribution[i];
        if (rnd >= x && rnd < y)
        {
            return i;
        }
    }
    raise("Didn't get result of probability distribution?");
}

void randInit()
{
    srand(time(0));
}

