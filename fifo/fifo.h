/**
 ***************************************************************
 * FIFO Implemetation
 * @author : Abel Gladstone mangam
 ***************************************************************
 **/
#ifndef FIFO_H__
#define FIFO_H__

/*
 * Include Files
 */
#include<stdint.h>
#include<stdbool.h>
#include "fifo_config.h"

/*
 * Defines
 */
#ifdef FIFO_USE_CUSTOM_MALLOC
typedef void* (*FIFOAllocFunction)(unsigned int sizeInBytes);
typedef void* (*FIFOFreeFunction)(void* ptr);
#else
#include<stdlib.h>
typedef void* FIFOAllocFunction;
typedef void* FIFOFreeFunction;
#endif

/*
 * Type definitions
 */
typedef enum {
    FIFO_NORMAL = 0,
    FIFO_FAST,
}FIFOType_t;
typedef struct {
   unsigned int head;
   unsigned int tail;
   void* storage;
   unsigned int unitSize;
   unsigned int maxElements;
   FIFOType_t type;
} FIFOBuffer_t;

/**
 ***************************************************************
 * Initializes the fifo object
 * params : 
 *  self        --> the pointer to the fifo object
 *  unitSize    --> the size of the each object in bytes 
 *                  including the padding bytes
 *  maxElements --> the maximum elements in the FIFO must 
 *                  be a power of 2
 *  fifoAlloc   --> the pointer to alloc function if 
 *                  a custom alloc is used in the fifo_config.h
 *                  or 0 
 * Returns : 
 *   true (1) is sucessful
 *   false (0) if failed
 ***************************************************************
 **/
bool FIFOInit( FIFOBuffer_t* self, const unsigned int unitSize, const unsigned int maxElements, FIFOType_t type, FIFOAllocFunction fifoAlloc );

/**
 ***************************************************************
 * Adds data to the fifo
 * params : 
 *  self        --> the pointer to the fifo object
 *  dataPtr     --> the pointer to the data to be added
 *  numElements --> the number of elements of each unitSize
 * Returns : 
 *  the number of successfully added elements in the Fifo
 *  returns 0 if the fifo is full
 ***************************************************************
 **/
unsigned int FIFOAddData( FIFOBuffer_t* self, void* dataPtr,const unsigned int numElements );

/**
 ***************************************************************
 * Reads data from the fifo
 * params : 
 *  self        --> the pointer to the fifo object
 *  dataPtr     --> the pointer to the data to be read to
 *  numElements --> the number of elements of each unitSize
 * Returns : 
 *  the number of successfully added elements in the Fifo
 *  returns 0 if the fifo is full
 ***************************************************************
 **/
unsigned int FIFOReadData( FIFOBuffer_t* self, void* dataPtr, const unsigned int numElements );

/**
 ***************************************************************
 * Reads data from the fifo but does not change the head or tail
 * of the buffer. i.e. the size of the buffer remains the same 
 * before and after the peek.
 * params : 
 *  self        --> the pointer to the fifo object
 *  dataPtr     --> the pointer to the data to be read to
 *  numElements --> the number of elements of each unitSize
 * Returns : 
 *  the number of successfully added elements in the Fifo
 *  returns 0 if the fifo is full
 ***************************************************************
 **/
unsigned int FIFOPeekData( FIFOBuffer_t* self, void* dataPtr, const unsigned int numElements );

/**
 ***************************************************************
 * Gets the currently filled size of the FIFO
 * params : 
 *  self        --> the pointer to the fifo object
 * Returns : 
 *  the number of elements currently unread in the fifo
 *  returns 0 if the fifo is empty
 ***************************************************************
 **/
unsigned int FIFOGetSize( FIFOBuffer_t* self );

/**
 ***************************************************************
 * Deinitializes the FIFO and frees the memory allocated
 * params : 
 *  self   --> the pointer to the fifo object
 *  freecB --> the callback to the free function can be 0
 * Returns : 
 *  void
 ***************************************************************
 **/
void FIFOClose( FIFOBuffer_t* self, FIFOFreeFunction freeCb );

/**
 ***************************************************************
 * Adds a null definition of the object to the fifo.
 * used to pre fill the buffer
 * params : 
 *  self          --> the pointer to the fifo object
 *  nullDefnition --> the pointer to know what a null is
 *                    needed as the implementation is type 
 *                    agnostic
 * numElements    --> the number of times the null element is to be added
 * Returns : 
 *  void
 ***************************************************************
 **/
void FIFOPrimeBuffer( FIFOBuffer_t* self, void* nullDefnition, const unsigned int numElements );




#endif /* FIFO_H__ */