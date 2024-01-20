#include "utils.h"


// My implementation of a generic vector

typedef struct
{
    void **items;
    int top;
    int capacity;
} Vector;

Vector *createNewVector(int capacity)
{
    Vector *newVector = (Vector *)malloc(sizeof(Vector));
    newVector->capacity = capacity;
    newVector->items = (void **)malloc(sizeof(void *) * capacity);
    newVector->top = -1;
    return newVector;
}

void resizeVector(Vector *vec)
{
    vec->capacity *= 2;
    vec->items = (void **)realloc(vec->items, sizeof(vec->items) * vec->capacity);
}

void push(Vector *vec, void *item)
{
    if (vec->capacity - 1 == vec->top)
    {
        resizeVector(vec);
    }
    vec->top++;
    vec->items[vec->top] = item;
}

void *pop(Vector *vec, int index)
{
    if (index > vec->top)
    {
        puts("Vector index out of range");
    }
    void *item = vec->items[index];
    for (int i = index; i < vec->top; i++) // changing the item list
    {
        vec->items[i] = vec->items[i + 1];
    }
    vec->top--;
    return item;
}

void freeVector(Vector *vec) // frees vector and its items from memory
{
    if (vec != NULL)
    {
        for (int i = 0; i <= vec->top; i++)
        {
            free(vec->items[i]);
        }
        free(vec->items);
        free(vec);
    }
}


void printVector(Vector *vec)
{
    if (vec->top == -1)
    {
        puts("Empty Vector");
    }
    char printStr[300];
    printStr[0] = '\0';
    sprintf(printStr, "Vector of items: ");
    for (int i = 0; i <= vec->top; i++)
    {
        void *item = vec->items[i];
        sprintf(printStr + strlen(printStr), "%s ", (char *)item);
    }
    puts(printStr);
}