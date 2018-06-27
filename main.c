#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include "fifo/fifo.h"
#include "lifo/lifo.h"

void* data;
LIFO_t lifo;

/* for some reason arrays are not supported */
typedef struct{
    uint32_t field1;
    uint32_t field2;
    uint8_t stringField[9];
}testStruct_t;

void print( LIFO_t* buffer, testStruct_t* startId, int len );

void printFifo(LIFO_t* self);

#define LIFO_MAX_SIZE   80

int main()
{
   testStruct_t a[LIFO_MAX_SIZE];
   testStruct_t b[LIFO_MAX_SIZE];
   int readSamples;
   const char name[] = "HELLO";
   char newName[9];
   if ( LIFOInit( &lifo, sizeof(testStruct_t), LIFO_MAX_SIZE, NULL ) == false )
   {
      return EXIT_FAILURE;
   }
   for ( int i = 0; i < LIFO_MAX_SIZE; i++ )
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
   printf("LIFO SIZE %d", LIFOGetSize(&lifo));
   LIFOPush(&lifo,a,LIFO_MAX_SIZE);
   printf("LIFO SIZE %d", LIFOGetSize(&lifo));
   readSamples = LIFOPop( &lifo, b, LIFO_MAX_SIZE);
   print( &lifo, b, readSamples );
   return EXIT_SUCCESS;

}

void print( LIFO_t* buffer, testStruct_t* startId, int len )
{
   printf( "------------------------------------------\n");
   for ( int i = 0; i < len; i++ )
   {
      if( strcmp( ( const char* )startId[i].stringField,"" ) != 0 )
      {
        printf( "%d.)%s\n", i,startId[i].stringField );
      }
      else
      {
        printf( "NULL\n" );
      }
   }
   printf( "------------------------------------------\n" );
   //printFifo(buffer);
}

void printFifo(LIFO_t* self)
{
   printf( "------------------------------------------\n" );
   printf( "HEAD : %d\n", self->head );
   printf( "SIZE : %d\n", LIFOGetSize( self ) );
   printf( "------------------------------------------\n" );
}
