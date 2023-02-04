#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include "../basicTypes.h"          //lint !e537

#ifndef EXT_RingBuffer
#define EXT_RingBuffer extern
#endif

typedef uint32_t rbElementSize_t;
typedef uint32_t rbNumElements_t;   /* Note, this MUST be an unsigned type due
                                     ** to implementation of getLast command. */
/**
 * The RingBuffer class manages a ring buffer of fixed sized elements.
 * Elements are input and output in a first in first out (FIFO) order.
 */
class RingBuffer
{
   public:
      /**
       * Constructs an empty ring buffer.
       */
      RingBuffer (void *pBuffer,
                  rbElementSize_t elementSize,
                  rbNumElements_t maxElements);

      /**
       * Release any resources consumed by the RingBuffer
       */
      virtual ~RingBuffer();

      /**
      * Put elements into the buffer. Returns the number of elements placed in the buffer.
      * The number of elements actually placed in the buffer may be less
      * than the requested number depending on space available in the
      * buffer.
      */
      virtual rbNumElements_t put (void *pSrcBuffer,
                                   rbNumElements_t numElements);

     /**
      * Get elements out of the buffer. Returns the number of elements actually
      * retrieved. The number of elements retrieved may be less than the requested
      * number depending on how many elements are in the buffer.
      */
      virtual rbNumElements_t get (void *pDstBuffer,
                                   rbNumElements_t numElements);

      /**
       * Get elements out of the buffer, starting with the last element put into
       * the buffer. This is backwards from how a normal ring buffer works. Returns
       * the number of elements actually retrieved. The number of elements retrieved may
       * be less than the requested number depending on how many elements are
       * in the buffer.
       */
      virtual rbNumElements_t getLast (void *pDstBuffer,
                                       rbNumElements_t numElements);

      /**
       * Returns true if there are no elements in the buffer.
       */
      virtual bool isEmpty (void);

      /**
       * Get statistics for the buffer.
       * @param pNumElements The number of element currently in the ring buffer.
       * @param pMaxElements The maximum number of elements that can be added to the ring buffer.
       * @param pHighWaterMark The most elements that have ever been in the ring buffer.
       */
      virtual void statistics (rbNumElements_t *pNumElements,
                               rbNumElements_t *pMaxElements,
                               rbNumElements_t *pHighWaterMark);

   private:
      void *pBuffer;
      rbElementSize_t elementSize;
      rbNumElements_t maxElements;
      rbNumElements_t numElements;
      rbNumElements_t in;
      rbNumElements_t out;
      rbNumElements_t highWaterMark;

      /**
       * Copy constructor for ring buffer. It
       * is currently not possible to copy a ring buffer, since allocation for
       * the memory for the buffer is not controlled by the class. Therefore the
       * copy constructor is defined and marked as private.
       */
      RingBuffer (const RingBuffer& rb __attribute__((unused)))
         : pBuffer(0),
           elementSize(0),
           maxElements(0),
           numElements(0),
           in(0),
           out(0),
           highWaterMark(0)
      { };     //lint !e1704

      /**
       * Assignment operator currently not implemented.
       */
      RingBuffer* operator=(RingBuffer& rb __attribute__((unused))) { return NULL; }; //lint !e1720

}; //lint !e1712

#endif
