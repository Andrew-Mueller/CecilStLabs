#include <cstdint>
#include <cstdlib>

#include "Memory.h"

void Memory::paint(int* startAddy, size_t numBytes)
{
    for (int* i = startAddy; i < (startAddy + numBytes); i++)
    {
        *i = (int)PAINT_VALUE;
    }
}

bool Memory::checkPaint(int* startAddy, size_t numBytes)
{
    bool success = true;

    for (int* i = startAddy; i < (startAddy + numBytes); i++)
    {
        if (PAINT_VALUE != *i)
        {
            success = false;
            break;
        }
    }

    return success;
}

uint16_t Memory::crc16(int* startAddy, size_t numBytes)
{
    // TODO
    return 0;
}

uint32_t Memory::crc32(int* startAddy, size_t numBytes)
{
    // TODO
    return 0;
}
