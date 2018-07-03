/**
 ***************************************************************
 * LIFO Implemetation
 * @author : AGM
 ***************************************************************
 **/
#include "lifo.h"

/*
 * Include Files
 */
#ifdef LIFO_USE_MEMCPY
#include<string.h>
#endif

/*
 * Private Function Declaration
 */
/**
 ***************************************************************
 * Adds an element to the lifo
 * params : 
 *  self   --> the pointer to the lifo object
 *  data   --> the pointer to the in data
 * Returns : 
 *  true if successful
 *  false when the lifo is full
 ***************************************************************
 **/
static bool LIFOPushElement( LIFO_t* self, const void* src );

/**
 ***************************************************************
 * Reads an element from the lifo
 * params : 
 *  self   --> the pointer to the lifo object
 *  data   --> the pointer to where the data should be read
 * Returns : 
 *  true if successful
 *  false when the lifo is empty
 ***************************************************************
 **/
static bool LIFOPopElement( LIFO_t* self, void* dest );

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
static void LIFOCopy(char* src, char* dest, unsigned int length );

/**
 ***************************************************************
 * Checks if the buffer is full
 * params : 
 *  self --> the pointer to the lifo object
 * Returns : 
 *  true  (1) if full
 *  false (0) otherwise
 ***************************************************************
 **/
static bool LIFOIsFull( LIFO_t* self );

/**
 ***************************************************************
 * Checks if the buffer is empty
 * params : 
 *  self --> the pointer to the lifo object
 * Returns : 
 *  true  (1) if empty
 *  false (0) otherwise
 ***************************************************************
 **/
static bool LIFOIsEmpty( LIFO_t* self );

/*
 * Public Functions Implementation
 */
/**************************************************************/
bool LIFOInit( LIFO_t* self, const unsigned int uintSize, const unsigned int maxElements, LIFOAllocFunction lifoAlloc )
{
    if( maxElements == 0 || uintSize == 0 )
    {
        return false;
    }
    self->head = 0;
    /* We start counting from 0 */
    self->maxSize = maxElements;
    self->unitSize = uintSize;
#ifdef LIFO_USE_CUSTOM_MALLOC
    self->storage = lifoAlloc( self->unitSize*self->maxSize );
#else
    self->storage = malloc( self->unitSize*self->maxSize );
#endif
    return !( self->storage == 0 );
}

/**************************************************************/
void LIFODeinit( LIFO_t* self, LIFOFreeFunction freeFunction )
{
#ifdef LIFO_USE_CUSTOM_MALLOC
    freeFunction(self->storage);
#else
    free(self->storage);
#endif
}

/**************************************************************/
unsigned int LIFOPush( LIFO_t* self, const void* data, const unsigned int numElements )
{
    char* src;
    unsigned int i;
    for(i = 0; i < numElements; i++ )
    {
        src = (char*)data + i*self->unitSize;
        if( !LIFOPushElement(self,src) )
        {
            return i;
        }
    }
    return i;
}

/**************************************************************/
unsigned int LIFOPop( LIFO_t* self, void* data, const unsigned int numElements )
{
    char* dest;
    int i;
    for(i = 0; i < numElements; i++ )
    {
        dest = (char*)data + i*self->unitSize;
        if( !LIFOPopElement(self, dest) )
        {
            return i;
        }
    }
    return i;
}

/**************************************************************/
unsigned int LIFOGetSize( LIFO_t* self )
{
    return self->head;
}

/*
 * Private Function Implementation
 */

/**************************************************************/
static bool LIFOPushElement( LIFO_t* self, const void* src )
{
    char* dest;
    if( LIFOIsFull(self) )
    {
        return false;
    }
    dest = (char*)self->storage + self->unitSize*self->head++;
    LIFOCopy( (char*)src, dest, self->unitSize );
    return true;
}

/**************************************************************/
static bool LIFOPopElement( LIFO_t* self, void* dest )
{
    char* src;
    if( LIFOIsEmpty( self ) )
    {
        return false;
    }
    src = (char*)self->storage + --self->head*self->unitSize;
    LIFOCopy( src, (char*)dest, self->unitSize );
    return true;
}

/**************************************************************/
static void LIFOCopy(char* src, char* dest, unsigned int length )
{
#ifdef LIFO_USE_MEMCPY
    memcpy( dest, src, length );
#else
    unsigned int i;
    for( i = 0; i < self->unitSize; i++ )
    {
        dest[i] = src[i];
    }
#endif
}

static bool LIFOIsFull( LIFO_t* self )
{
    return (self->head == self->maxSize);
}

static bool LIFOIsEmpty( LIFO_t* self )
{
    return (self->head == 0);
}