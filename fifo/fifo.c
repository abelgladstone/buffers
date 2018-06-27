/**
 ***************************************************************
 * FIFO Implemetation
 * @author : AGM
 ***************************************************************
 **/

/*
 * Include Files
 */
#include "fifo.h"
#ifdef FIFO_USE_MEMCPY
#include<string.h>
#endif

/*
 * Type Definitions
 */
typedef enum
 {
    FIFO_HEAD = 0,
    FIFO_TAIL
}FIFOPointer_t;

/*
 * Private Function Declaration
 */

/**
 ***************************************************************
 * Adds an element to the fifo
 * params : 
 *  self   --> the pointer to the fifo object
 *  inData --> the pointer to the in data
 * Returns : 
 *  void
 ***************************************************************
 **/
static void FIFOAddElement( FIFO_t* self, void* inData );

/**
 ***************************************************************
 * Reads an element from the fifo
 * params :
 *  self    --> the pointer to the fifo object
 *  outData --> the pointer to place the read data
 * Returns : 
 *  void
 ***************************************************************
 **/
static void FIFOReadElement( FIFO_t* self, void* outData );

/**
 ***************************************************************
 * Checks if the buffer is full
 * params : 
 *  self --> the pointer to the fifo object
 * Returns : 
 *  true  (1) if full
 *  false (0) otherwise
 ***************************************************************
 **/
static bool FIFOIsFull( FIFO_t* self );

/**
 ***************************************************************
 * Checks if the buffer is empty
 * params : 
 *  self --> the pointer to the fifo object
 * Returns : 
 *  true  (1) if empty
 *  false (0) otherwise
 ***************************************************************
 **/
static bool FIFOIsEmpty( FIFO_t* self );

/**
 ***************************************************************
 * Copies data from a src to the destination
 * similar to the memcpy.
 * params : 
 *  src --> the pointer to the source
 *  dest --> the pointer to the destination
 * Returns :
 *  void
 ***************************************************************
 **/
static void FIFOCopy(const char* src, char* dest, int length );

/**
 ***************************************************************
 * Increments the pointer of the fifo either head or tail
 * params : 
 *  self --> the pointer to the fifo buffer
 *  head --> is either FIFO_HEAD or FIFO_TAIL
 * Returns :
 *  the pointer to store or read the next element
 ***************************************************************
 **/
static char* FIFOIncrementPointer( FIFO_t* self, FIFOPointer_t head );


/*
 * Public Functions Implementation
 */
/**************************************************************/
bool FIFOInit( FIFO_t* self, const unsigned int unitSize, const unsigned int maxElements, FIFOType_t type, FIFOAllocFunction fifoAlloc )
{
    if ( type == FIFO_FAST )
    {
        if( (maxElements & (maxElements - 1)) != 0 )
        {
            return false;
        }
    }
    self->type = type;
    self->head = 0;
    self->tail = 0;
    /* We are counting from zero */
    self->maxElements = maxElements - 1;
    self->unitSize = unitSize;
#ifdef FIFO_USE_CUSTOM_MALLOC
    self->storage = fifoAlloc(self->unitSize*self->maxElements);
#else
    self->storage = malloc(self->unitSize*self->maxElements);
#endif
    if (self->storage == 0)
    {
        return false;
    }
    return true;
}

/**************************************************************/
unsigned int FIFOAddData( FIFO_t* self, void* dataPtr, const unsigned int numElements )
{
   int i;
   for ( i = 0; i < numElements; i++ )
   {
      if ( FIFOIsFull( self ) )
      {
         return i;
      }
      FIFOAddElement( self, dataPtr + i * self->unitSize );
   }
   return i;
}

/**************************************************************/
unsigned int FIFOGetSize(FIFO_t* self)
{
    return self->head - self->tail;
}

/**************************************************************/
unsigned int FIFOReadData( FIFO_t* self, void* dataPtr, const unsigned int numElements )
{
   int i;
   for ( i = 0; i < numElements; i++ )
   {
      if ( FIFOIsEmpty(self) )
      {
         return i;
      }
      FIFOReadElement( self, dataPtr + i * self->unitSize );
   }
   return i;
}

unsigned int FIFOPeekData( FIFO_t* self, void* dataPtr, const unsigned int numElements )
{
   int i,j;
   char* src;
   char* dest;
   unsigned int offset;
   unsigned int elementsToRead;
   dest = dataPtr;
   elementsToRead = ( FIFOGetSize( self ) > numElements )?numElements: FIFOGetSize( self );
   for ( i = 0, j = self->tail; i < elementsToRead; i++,j++ )
   {
      offset = ( self->type )?( j & self->maxElements ) : (j % (self->maxElements + 1) );
      src = (char*)self->storage + self->unitSize*offset;
      dest = (char*)dataPtr + self->unitSize*i;
      FIFOCopy( src, dest, self->unitSize );
   }
   return i;
}

/**************************************************************/
void FIFODeInit( FIFO_t* self, FIFOFreeFunction freeCb )
{
#ifdef FIFO_USE_CUSTOM_MALLOC
   freeCb(self->storage);
#else
   free(self->storage);
#endif
}

/**************************************************************/
void FIFOPrimeBuffer( FIFO_t* self, void* nullDefnition, const unsigned int numElements )
{
    unsigned int length;
    length = ( numElements > (self->maxElements + 1))? (self->maxElements+1): numElements;
    for( int i = 0; i < length; i++ )
        FIFOAddElement( self, nullDefnition );
}

/*
 * Private Function Implementation
 */
/**************************************************************/
static void FIFOReadElement(FIFO_t* self, void* outData)
{
   char* src;
   src = FIFOIncrementPointer( self, FIFO_TAIL );
   FIFOCopy( src, outData, self->unitSize );
}

/**************************************************************/
static void FIFOAddElement(FIFO_t* self, void* inData)
{
   char* dest;
   dest = FIFOIncrementPointer( self, FIFO_HEAD  );
   FIFOCopy( inData, dest, self->unitSize );
}

/**************************************************************/
static void FIFOCopy(const char* src, char* dest, int length)
{
#ifdef FIFO_USE_MEMCPY
    memcpy( dest, src, length );
#else
   int i;
   for( i = 0; i < length; i++ )
   {
      dest[i] = src[i];
   }
#endif
}

/**************************************************************/
static char* FIFOIncrementPointer( FIFO_t* self, FIFOPointer_t head )
{
   unsigned int* ptr;
   unsigned int offset;
   ptr = ( head )?( &self->tail ):( &self->head );
   offset = ( self->type )?( *ptr & self->maxElements ) : (*ptr % (self->maxElements + 1) );
   *ptr += 1;
   return (char*)( self->storage + offset * self->unitSize );
}

/**************************************************************/
static bool FIFOIsFull( FIFO_t* self )
{
   return ( ( self->head - self->tail ) == ( self->maxElements + 1 ) );
}

/**************************************************************/
static bool FIFOIsEmpty( FIFO_t* self )
{
   return ( ( self->head - self->tail ) == 0 );
}
