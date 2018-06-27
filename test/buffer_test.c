#include "buffer_test.h"
#include<string.h>
#include<stdio.h>

static TEST_t inputData[MAX_LENGTH];
static TEST_t outputData[MAX_LENGTH];
static void STUBInit();
static void STUBDeInit();
static void print( TEST_t* self, int numSamples );
static const TEST_t zeroData = {.a = 0,
                                .b = 0.0, 
                                .c = 0.0, 
                                .name = { 0,0,0,0,
                                          0,0,0,0,
                                          0,0,0,0}};
const char ANAME[] = "HELLO";
static FIFO_t fifo;
static LIFO_t lifo;

bool TESTFIFO()
{
    bool retVal = true;
    STUBInit();
    retVal &= FIFOInit( &fifo, sizeof(TEST_t), MAX_LENGTH, FIFO_NORMAL, 0);
    FIFOPrimeBuffer( &fifo, (void*)&zeroData, 10 );
    retVal &= ( FIFOGetSize( &fifo ) == 10 );
    retVal &= ( FIFOAddData( &fifo, (void*)inputData, 10 ) == 10 );
    retVal &= ( FIFOGetSize( &fifo ) == MAX_LENGTH );
    retVal &= ( FIFOAddData( &fifo, (void*)&inputData[10], 1) == 0 );
    retVal &= ( FIFOReadData( &fifo, outputData, 10) == 10 );
    for( int i = 0; i < 10; i++ )
    {
        retVal &= (outputData[i].a == 0 );
        retVal &= (outputData[i].b == 0.0);
        retVal &= (outputData[i].c == 0.0);
        retVal &= ( strcmp(outputData[i].name,"") == 0 );
    }
    retVal &= ( FIFOGetSize( &fifo ) == 10 );
    retVal &= ( FIFOPeekData( &fifo, outputData, 10 ) == 10 );
    for( int i = 0; i < 10; i++ )
    {
        char expectedString[12];
        retVal &= (outputData[i].a == i );
        retVal &= (outputData[i].b == (i+1.0f)*1.2f);
        retVal &= (outputData[i].c == 1.2f);
        sprintf( expectedString,"%s-%d",ANAME,i );
        retVal &= ( strcmp(outputData[i].name,expectedString ) == 0 );
    }
    retVal &= ( FIFOGetSize( &fifo ) == 10 );
    retVal &= ( FIFOReadData( &fifo, outputData, 10) == 10 );
    for( int i = 0; i < 10; i++ )
    {
        char expectedString[12];
        retVal &= (outputData[i].a == i );
        retVal &= (outputData[i].b == (i+1.0f)*1.2f);
        retVal &= (outputData[i].c == 1.2f);
        sprintf( expectedString,"%s-%d",ANAME,i );
        retVal &= ( strcmp(outputData[i].name,expectedString ) == 0 );
    }
    retVal &= ( FIFOGetSize( &fifo ) == 0 );
    STUBDeInit();
    return retVal;
}

bool TESTLIFO()
{
    bool retVal = true;
    STUBInit();
    retVal &= LIFOInit( &lifo, sizeof(TEST_t), MAX_LENGTH, 0 );
    retVal &= ( LIFOPush( &lifo, inputData, 10 ) == 10 );
    retVal &= ( LIFOGetSize( &lifo ) == 10 );
    retVal &= ( LIFOPop( &lifo, outputData, 5 ) == 5 );
    int startVal = 9;
    char expectedString[12];
    for( int i = 0; i < 5; i++ )
    {
        retVal &= ( outputData[i].a == startVal );
        retVal &= ( outputData[i].b == (startVal + 1.0f)*1.2f );
        retVal &= ( outputData[i].c == 1.2f );
        sprintf( expectedString, "%s-%d", ANAME,startVal );
        retVal &= ( strcmp( outputData[i].name,expectedString ) == 0 );
        startVal--;
    }
    retVal &= ( LIFOGetSize( &lifo ) == 5 );
    return retVal;
}

static void STUBInit()
{
    int i;
    for( i = 0; i < MAX_LENGTH; i++ )
    {
        inputData[i].a = i;
        inputData[i].b = (float)(i + 1.0f)*1.2f;
        inputData[i].c = inputData[i].b/(float)(inputData[i].a + 1.0f);
        sprintf( inputData[i].name, "%s-%d",ANAME,i);
    }
}

static void STUBDeInit()
{
    for(int i = 0; i < MAX_LENGTH; i++ )
    {
        memcpy(&inputData[i],&zeroData,sizeof(TEST_t));
    }
}

static void print( TEST_t* self, int numSamples )
{
   printf( "------------------------------------------\n");
   for ( int i = 0; i < numSamples; i++ )
   {
    printf("-----\n");
    printf( "INDEX : %d\nDATA  : %.2f\nDATA  : %.2f\nNAME  : %s\n", self[i].a, self[i].b, self[i].c, self[i].name);
   }
   printf( "------------------------------------------\n" );
}