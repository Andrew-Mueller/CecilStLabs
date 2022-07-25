/**
 * The MemoryPool is a list of free memory chunks of a specified type.
 * When a chunk is needed, it can be allocated from this list. When the chunk
 * has been processed it should be put back in the pool. Using a MemoryPool is
 * done to avoid dynamic memory allocation.
 *
 * NOTE:
 * As described in the C++ specification, a template class must exist all in
 * the header file.  This is because the template class is only created at run
 * time, and not compile time.
 */


#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include "../basicTypes.h"                         //lint !e537
#include "../LinkedList/SingleLinkedList.h"

template <class Type> class MemoryPool : private SingleLinkedList
{
   public:

      /**
       * Constructor which creates a pool from an array of objects. This is
       * the simplest way to create a pool of objects. Simply define a global
       * array of objects, then pass a pointer to that array into this constructor.
       */
      MemoryPool (Type *pChunks = NULL, sllListLength_t numChunks = 0);

      /**
       * Allocate a <Type> from the pool. Returns NULL
       * if the pool is empty.
       */
      Type * allocate (void);

      /**
       * Return a <Type> to the pool.
       * @param pChunk the value to return to the memory pool.
       */
      void free (Type *pChunk);

   private:
      /**
       * MemoryPool (const MemoryPool&):: Copy constructor, which is defined
       * private, because pools can not be copied.
       */
      MemoryPool (const MemoryPool &pool) {};
      MemoryPool& operator= (MemoryPool& pool) { return NULL; };
};

template <class Type>
MemoryPool<Type>::MemoryPool (Type *pChunks,
                              sllListLength_t numChunks)
    : SingleLinkedList()
{
    sllListLength_t i;

    if (pChunks != NULL)
    {
        for (i = 0; i < numChunks; i++)
        {
           (void)insert ((SingleLink *)&pChunks[i]);
        }
    }
}

template <class Type>
Type * MemoryPool<Type>::allocate (void)
{
    return ((Type *)remove());
}

template <class Type>
void MemoryPool<Type>::free (Type *pChunk)
{
    (void)insert ((SingleLink *)pChunk);     //lint !e740
}


#endif
