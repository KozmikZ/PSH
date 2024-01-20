#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void printInt(int toPrint)
{
    printf("%d\n", toPrint);
}

void raise(char *strExc)
{
    printf("ERROR: %s\n",strExc);
    exit(EXIT_FAILURE);
}

void assert(bool condition, char *strExc){
    if(!condition){
        raise(strExc);
    }
}

int above_0(int x){
    if(x<0){
        return 0;
    }
    else{
        return x;
    }
}

#endif

