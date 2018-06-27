/**
 ***************************************************************
 * LIFO Implemetation
 * @author : AGM
 ***************************************************************
 **/
#ifndef LIFO_H__
#define LIFO_H__

/*
 * Include Files
 */
#include<stdint.h>
#include<stdbool.h>
#include "lifo_config.h"

#ifndef LIFO_USE_CUSTOM_MALLOC
#include<stdlib.h>
#endif

/*
 * Type Definitions
 */
typedef struct {
    unsigned int head;
    unsigned int maxSize;
    unsigned int unitSize;
    void* storage;
}LIFO_t;

#ifndef LIFO_USE_CUSTOM_MALLOC
typedef void* LIFOAllocFunction;
typedef void* LIFOFreeFunction;
#else
typedef void* (*LIFOAllocFunction)(unsigned int sizeInBytes);
typedef void* (*LIFOFreeFunction)(void* ptr);
#endif

/**
 ***************************************************************
 * Initializes the lifo object
 * params : 
 *  self        --> the pointer to the lifo object
 *  unitSize    --> the size of the each object in bytes 
 *                  including the padding bytes
 *  maxElements --> the maximum elements in the FIFO must 
 *                  be a power of 2
 *  lifoAlloc   --> the pointer to alloc function if 
 *                  a custom alloc is used in the lifo_config.h
 *                  or 0 
 * Returns : 
 *   true (1) is sucessful
 *   false (0) if failed
 ***************************************************************
 **/
bool LIFOInit( LIFO_t* self, const unsigned int uintSize, const unsigned int maxElements, LIFOAllocFunction lifoAlloc );

/**
 ***************************************************************
 * Deinitializes the LIFO and frees the memory allocated
 * params : 
 *  self   --> the pointer to the lifo object
 *  freecB --> the callback to the free function can be 0
 * Returns : 
 *  void
 ***************************************************************
 **/
void LIFODeInit( LIFO_t* self, LIFOFreeFunction freecB );

/**
 ***************************************************************
 * Adds data to the lifo
 * params : 
 *  self        --> the pointer to the lifo object
 *  data        --> the pointer to the data to be added
 *  numElements --> the number of elements of each unitSize
 * Returns : 
 *  the number of successfully added elements in the lifo
 *  returns 0 if the lifo is full
 ***************************************************************
 **/
unsigned int LIFOPush( LIFO_t* self, const void* data, const unsigned int numElements );

/**
 ***************************************************************
 * Reads data from the lifo
 * params : 
 *  self        --> the pointer to the lifo object
 *  data        --> the pointer to the data to be read to
 *  numElements --> the number of elements of each unitSize
 * Returns : 
 *  the number of successfully added elements in the lifo
 *  returns 0 if the lifo is full
 ***************************************************************
 **/
unsigned int LIFOPop( LIFO_t* self, void* data, const unsigned int numElements );

/**
 ***************************************************************
 * Gets the currently filled size of the LIFO
 * params : 
 *  self        --> the pointer to the lifo object
 * Returns : 
 *  the number of elements currently in the LIFO
 *  returns 0 if the fifo is empty
 ***************************************************************
 **/
unsigned int LIFOGetSize( LIFO_t* self );


#endif /* LIFO_H__ */