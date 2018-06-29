#include "buffer_test.h"
#include<string.h>
#include<stdio.h>

#define PI (double)3.14159

static TEST_t inputData[MAX_LENGTH];
static TEST_t outputData[MAX_LENGTH];
static void STUBInit();
static void STUBDeInit();
static void print( TEST_t* self, int numSamples );
static TEST_t zeroData = {.a = 0,
                                .b = 0.0, 
                                .c = 0.0, 
                                .name = { 0,0,0,0,
                                          0,0,0,0,
                                          0,0,0,0}};
const char ANAME[] = "HELLO";
static FIFO_t fifo;
static LIFO_t lifo;

typedef enum{
    CHECK_INT_UNSIGNED,
    CHECK_INT_SIGNED,
    CHECK_FLOAT,
    CHECK_DOUBLE,
    CHECK_STRING,
    CHECK_BOOLEAN,
}DATATYPE_t;

static bool CHECK_EQUAL( void* inputValue, void* expectedValue, DATATYPE_t type, const char* testName );

bool TESTFIFO()
{
    bool trueResult = true;
    unsigned int zero = 0;
    bool falseResult = false;
    bool retVal = true;
    unsigned int sizeUsed = 10;
    unsigned int peekSize = 5;
    unsigned int maxSize = MAX_LENGTH;
    unsigned int size;
    STUBInit();
    retVal &= FIFOInit( &fifo, sizeof(TEST_t), MAX_LENGTH, FIFO_NORMAL, 0);
    retVal &= CHECK_EQUAL( &retVal, &trueResult, CHECK_BOOLEAN, "Initialization Test" );
    FIFOPrimeBuffer( &fifo, (void*)&zeroData, sizeUsed );
    size = FIFOGetSize( &fifo );
    retVal &= CHECK_EQUAL( &size, &sizeUsed, CHECK_INT_UNSIGNED, "Size after priming" );
    size = FIFOAddData( &fifo, (void*)inputData, sizeUsed );
    retVal &= CHECK_EQUAL( &size, &sizeUsed, CHECK_INT_UNSIGNED, "Adding elements" );
    size = FIFOGetSize( &fifo );
    retVal &= CHECK_EQUAL( &size, &maxSize, CHECK_INT_UNSIGNED, "Size after adding data" );
    size = FIFOAddData( &fifo, (void*)&inputData[10], 1);
    retVal &= CHECK_EQUAL( &size, &zero, CHECK_INT_UNSIGNED, "Adding data when the buffer is full" );
    size = FIFOReadData( &fifo, outputData, sizeUsed);
    retVal &= CHECK_EQUAL( &size, &sizeUsed, CHECK_INT_UNSIGNED, "Reading data" );
    for( int i = 0; i < sizeUsed; i++ )
    {
        retVal &= CHECK_EQUAL( &outputData[i].a, &zeroData.a, CHECK_INT_UNSIGNED, "" );
        retVal &= CHECK_EQUAL( &outputData[i].b, &zeroData.b, CHECK_FLOAT, "" );
        retVal &= CHECK_EQUAL( &outputData[i].c, &zeroData.c, CHECK_DOUBLE, "" );
        retVal &= CHECK_EQUAL( outputData[i].name, zeroData.name, CHECK_STRING, "" );
    }
    size = FIFOGetSize( &fifo );
    retVal &= CHECK_EQUAL( &size, &sizeUsed, CHECK_INT_UNSIGNED, "Size after reading" );
    size = FIFOPeekData( &fifo, outputData, peekSize );
    retVal &= CHECK_EQUAL( &size, &peekSize, CHECK_INT_UNSIGNED, "Peeking" );
    for( int i = 0; i < peekSize; i++ )
    {
        char expectedString[12];
        float expVal = ((float)i+1.0f)*1.2f;
        double expDouble = PI;
        sprintf( expectedString,"%s-%d",ANAME,i );
        retVal &= CHECK_EQUAL( &outputData[i].a, &i , CHECK_INT_SIGNED, "" );
        retVal &= CHECK_EQUAL( &outputData[i].b, &expVal, CHECK_FLOAT, "" );
        retVal &= CHECK_EQUAL( &outputData[i].c, &expDouble, CHECK_DOUBLE, "" );
        retVal &= CHECK_EQUAL( outputData[i].name, expectedString, CHECK_STRING, "" );
    }
    size = FIFOGetSize( &fifo );
    retVal &= CHECK_EQUAL( &size, &sizeUsed, CHECK_INT_UNSIGNED, "Size after peeking" );
    size = FIFOReadData( &fifo, outputData, sizeUsed);
    retVal &= CHECK_EQUAL( &size, &sizeUsed, CHECK_INT_UNSIGNED, "Reading again" );
    for( int i = 0; i < sizeUsed; i++ )
    {
        char expectedString[12];
        float expVal = ((float)i+1.0f)*1.2f;
        double expDouble = PI;
        sprintf( expectedString,"%s-%d",ANAME,i );
        retVal &= CHECK_EQUAL( &outputData[i].a, &i , CHECK_INT_SIGNED, "" );
        retVal &= CHECK_EQUAL( &outputData[i].b, &expVal, CHECK_FLOAT, "" );
        retVal &= CHECK_EQUAL( &outputData[i].c, &expDouble, CHECK_DOUBLE, "" );
        retVal &= CHECK_EQUAL( outputData[i].name, expectedString, CHECK_STRING, "" );
    }
    size = FIFOGetSize( &fifo );
    retVal &= CHECK_EQUAL( &size, &falseResult, CHECK_INT_UNSIGNED, "checking the size after reading" );
    STUBDeInit();
    return retVal;
}

bool TESTLIFO()
{
    bool retVal = true;
    unsigned int sizeUsed = 10;
    unsigned int popSizeUsed = 5;
    unsigned int size;
    bool trueResult = true;
    STUBInit();
    retVal &= LIFOInit( &lifo, sizeof(TEST_t), MAX_LENGTH, 0 );
    retVal &= CHECK_EQUAL( &retVal, &trueResult, CHECK_BOOLEAN, "Initialising LIFO test");
    size = LIFOPush( &lifo, inputData, sizeUsed );
    retVal &= CHECK_EQUAL( &size, &sizeUsed, CHECK_INT_UNSIGNED, "Pushing to Lifo" );
    size = LIFOGetSize( &lifo );
    retVal &= CHECK_EQUAL( &size, &sizeUsed, CHECK_INT_UNSIGNED, "Reading the size of the LIFO" );
    size = LIFOPop( &lifo, outputData, popSizeUsed );
    retVal &= CHECK_EQUAL( &size, &popSizeUsed, CHECK_INT_UNSIGNED, "Poping from the LIFO" );
    int startVal = 9;
    char expectedString[12];
    for( int i = 0; i < 5; i++ )
    {
        float expectedFloat = (startVal + 1.0f)*1.2f;
        double expectedDouble = PI;
        sprintf( expectedString, "%s-%d", ANAME,startVal );
        retVal &= CHECK_EQUAL( &outputData[i].a, &startVal, CHECK_INT_UNSIGNED, "" );
        retVal &= CHECK_EQUAL( &outputData[i].b, &expectedFloat, CHECK_FLOAT, "" );
        retVal &= CHECK_EQUAL( &outputData[i].c, &expectedDouble, CHECK_DOUBLE, "" );
        retVal &= CHECK_EQUAL( outputData[i].name, expectedString, CHECK_STRING, "" );
        startVal--;
    }
    size = LIFOGetSize( &lifo );
    unsigned int expectedSize = sizeUsed - popSizeUsed;
    retVal &= CHECK_EQUAL( &size, &expectedSize, CHECK_INT_UNSIGNED, "Checking the remaing elements in the LIFO") ;
    return retVal;
}

static void STUBInit()
{
    int i;
    for( i = 0; i < MAX_LENGTH; i++ )
    {
        inputData[i].a = i;
        inputData[i].b = (float)(i + 1.0f)*1.2f;
        inputData[i].c = PI;
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

static bool CHECK_EQUAL( void* inputValue, void* expectedValue, DATATYPE_t type, const char* testName )
{
    switch(type)
    {
        case CHECK_INT_UNSIGNED:
        {
            unsigned int i,e;
            i = *(unsigned int *)inputValue;
            e = *(unsigned int *)expectedValue;
            if( i != e )
            {
                if( strlen(testName) != 0)
                    printf( "Error in Unsigned int comparison in %s:-", testName );
                printf( "Expected Value is %u, the input is %u\n", e, i );
                return false;
            }
            else
            {
                if( strlen(testName) != 0)
                    printf( "%s is successful\n", testName );
                return true;
            }
            
        }
        break;
        case CHECK_INT_SIGNED:
        {
            int i,e;
            i = *(int *)inputValue;
            e = *(int *)expectedValue;
            if( i != e )
            {
                if( strlen(testName) != 0)
                    printf( "Error in int comparison in %s:-", testName );
                printf("Expected Value is %d, the input is %d\n", e, i );
                return false;
            }
            else
            {
                if( strlen(testName) != 0)
                    printf( "%s is successful\n", testName );
                return true;
            }
        }
        break;
        case CHECK_FLOAT:
        {
            float i,e;
            i = *(float *)inputValue;
            e = *(float *)expectedValue;
            if( i != e )
            {
                if( strlen(testName) != 0)
                    printf( "Error in float comparison in %s:-", testName );
                printf( "Expected Value is %f, the input is %f\n", e, i );
                return false;
            }
            else
            {
                if( strlen(testName) != 0)
                    printf( "%s is successful\n", testName );
                return true;
            }
        }
        break;
        case CHECK_DOUBLE:
        {
            double i,e;
            i = *(double *)inputValue;
            e = *(double *)expectedValue;
            if( i != e )
            {
                if( strlen(testName) != 0)
                    printf( "Error in double comparison in %s:-", testName );
                printf( "Expected Value is %lf, the input is %lf\n", e, i );
                return false;
            }
            else
            {
                if( strlen(testName) != 0)
                    printf( "%s is successful\n", testName );
                return true;
            }
        }
        break;
        case CHECK_STRING:
        {
            char *i,*e;
            i = (char *)inputValue;
            e = (char *)expectedValue;
            if( strcmp(i,e) != 0 )
            {
                if( strlen(testName) != 0)
                    printf( "Error in string comparison in %s:-", testName );
                printf("Expected Value is %s, the input is %s\n", e, i );
                return false;
            }
            else
            {
                if( strlen(testName) != 0)
                    printf( "%s is successful\n", testName );
                return true;
            }
        }
        case CHECK_BOOLEAN:
        {
            bool i,e;
            i = *(bool*)inputValue;
            e = *(bool*)expectedValue;
            if( i != e )
            {
                if( strlen(testName) != 0)
                    printf( "Error in boolean comparison in %s:-", testName );
                printf("Expected Value is %d, the input is %d\n", (int)e, (int)i );
                return false;
            }
            else
            {
                if( strlen(testName) != 0)
                    printf( "%s is successful\n", testName );
                return true;
            }
        }
    }
    return false;
}