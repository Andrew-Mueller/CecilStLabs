#define EXT_RingBuffer
#include "RingBuffer.h"

RingBuffer::RingBuffer (void *pBuffer,
                        rbElementSize_t elementSize,
                        rbNumElements_t maxElements)
{
    this->pBuffer = pBuffer;
    if (pBuffer != NULL)
    {
        this->elementSize = elementSize;
        this->maxElements = maxElements;
    }
    else
    {
        this->elementSize = 0;
        this->maxElements = 0;
    }

    numElements = 0;
    in = 0;
    out = 0;
    highWaterMark = 0;
}

RingBuffer::~RingBuffer()
{
   // intentionally left blank.
}

rbNumElements_t RingBuffer::put (void *pSrcBuffer, rbNumElements_t numElements)
{
    rbNumElements_t numAdded = 0;
    rbElementSize_t i;
    uint8_t *pDest;

    // While the buffer is not full, and numElements is > 0
    while (this->numElements < maxElements && numElements > 0)
    {
        /* Copy source element into buffer */
        pDest = (uint8_t*)pBuffer + (elementSize * in);
        for (i = 0; i < elementSize; ++i)
        {
            *pDest = *((uint8_t*)pSrcBuffer);
            ++pDest;
            pSrcBuffer = (void *)((uint8_t *)pSrcBuffer + 1);
        }

        /* Adjust indices */
        --numElements;

        ++numAdded;

        ++(this->numElements);
        if (this->numElements > highWaterMark)
        {
            highWaterMark = this->numElements;
        }

        ++in;
        if (in >= maxElements)
        {
            in = 0;
        }
    }

    return (numAdded);
}

rbNumElements_t RingBuffer::get (void *pDstBuffer, rbNumElements_t numElements)
{
    rbNumElements_t numRetrieved = 0;
    rbElementSize_t i;
    uint8_t *pSrc;

    // While the buffer is not empty, and numElements is > 0)
    while (this->numElements > 0 && numElements > 0)
    {
        // Copy buffer element into destination
        pSrc = (uint8_t*)pBuffer + (elementSize * out);
        for (i = 0; i < elementSize; ++i)
        {
            *((uint8_t*)pDstBuffer) = *pSrc;
            pDstBuffer = (void *)((uint8_t *)pDstBuffer + 1);
            ++pSrc;
        }

        // Adjust indices
        --numElements;

        ++numRetrieved;

        --(this->numElements);

        ++out;
        if (out >= maxElements)
        {
            out = 0;
        }
    }

    return (numRetrieved);
}

rbNumElements_t RingBuffer::getLast (void *pDstBuffer, rbNumElements_t numElements)
{
    rbNumElements_t numRetrieved = 0;
    rbElementSize_t i;
    uint8_t *pSrc;

    // While the buffer is not empty, and numElements is > 0)
    while (this->numElements > 0 && numElements > 0)
    {
        // Back up in to the last element inserted
        --in;
        if (in >= maxElements)
            in = maxElements - 1;

        // Copy buffer element into destination
        pSrc = (uint8_t*)pBuffer + (elementSize * in);
        for (i = 0; i < elementSize; ++i)
        {
            *((uint8_t*)pDstBuffer) = *pSrc;
            pDstBuffer = (void *)((uint8_t *)pDstBuffer + 1);
            ++pSrc;
        }

        // Adjust element numbers
        --numElements;

        ++numRetrieved;

        --(this->numElements);
    }

    return (numRetrieved);
}

bool RingBuffer::isEmpty()
{
   return (numElements == 0 ? true : false);
}

void RingBuffer::statistics (rbNumElements_t *pNumElements,
                             rbNumElements_t *pMaxElements,
                             rbNumElements_t *pHighWaterMark)
{
    *pNumElements = numElements;
    *pMaxElements = maxElements;
    *pHighWaterMark = highWaterMark;
}
