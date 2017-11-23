#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include "fifo/fifo.h"

void* data;
FIFOBuffer_t fifo;

/* for some reason arrays are not supported */
typedef struct{
    uint32_t field1;
    uint32_t field2;
    uint8_t stringField[9];
}testStruct_t;

void print( FIFOBuffer_t* buffer, testStruct_t* startId, int len );

void printFifo(FIFOBuffer_t* self);

#define FIFO_MAX_SIZE   16

int main()
{
   testStruct_t a[FIFO_MAX_SIZE];
   testStruct_t b[FIFO_MAX_SIZE];
   int readSamples;
   const char name[] = "HELLO";
   char newName[9];
   if ( FIFOInit( &fifo, sizeof(testStruct_t), FIFO_MAX_SIZE, FIFO_FAST, NULL ) == false )
   {
      return EXIT_FAILURE;
   }
   for ( int i = 0; i < FIFO_MAX_SIZE; i++ )
   {
      a[i].field1 = i;
      a[i].field2 = i*i;
      sprintf(newName,"%s-%d",name,i);
      strcpy((char*)a[i].stringField,newName);
      b[i].field1 = 0;
      b[i].field2 = 0;
      for( int j = 0; j < 9; j++ )
      {
          b[i].stringField[j] = '\0';
      }
   }
   FIFOAddData( &fifo, a, 4 );
   for(int i = 0; i < 5; i++ )
   {
    readSamples = FIFOPeekData( &fifo, b, 4 );
    print( &fifo, b, readSamples );
   }
   return EXIT_SUCCESS;

}

void print( FIFOBuffer_t* buffer, testStruct_t* startId, int len )
{
   //printf( "------------------------------------------\n");
   for ( int i = 0; i < len; i++ )
   {
      if( strcmp((const char*)startId[i].stringField,"") != 0 )
      {
        printf( "%d.)%s\n", i,startId[i].stringField );
      }
      else
      {
        printf("NULL\n");
      }
   }
   //printf( "------------------------------------------\n");
   //printFifo(buffer);
}

void printFifo(FIFOBuffer_t* self)
{
   printf( "------------------------------------------\n");
   printf( "HEAD : %d\n", self->head );
   printf( "TAIL : %d\n", self->tail );
   printf( "SIZE : %d\n", FIFOGetSize( self ) );
   printf( "------------------------------------------\n");
}
