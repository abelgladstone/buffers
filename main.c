#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include "buffer_test.h"

int main()
{
   if( TESTFIFO() == false )
   {
       printf("FIFO Test Failed\n");
       return EXIT_FAILURE;
   }
   else
   {
    printf("FIFO Test Successful\n");
   }
   if( TESTLIFO() == false )
   {
       printf("LIFO Test Failed\n");
       return EXIT_FAILURE;
   }
   else
   {
    printf("LIFO Test Successful\n");
   }
   return EXIT_SUCCESS;
}