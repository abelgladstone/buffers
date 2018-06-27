#ifndef BUFFER_TEST_H__
#define BUFFER_TEST_H__

#include<stdbool.h>
#include "fifo.h"
#include "lifo.h"

#define MAX_LENGTH  20

typedef struct
{
    int a;
    float b;
    double c;
    char name[12];
}TEST_t;

bool TESTFIFO();

bool TESTLIFO();

#endif /* BUFFER_TEST_H__ */