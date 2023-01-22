#ifndef MEMORY_H
#define MEMORY_H

/**
 * Collection of operations that operate on a region of memory.
 */
class Memory
{
    public:

        /**
         * Fill the given memory range with a predictable value.
         *
         * @param startAddy The address to start painting
         * @param numBytes The number of bytes to paint
         */
        static void paint(int* startAddy, size_t numBytes);

        /**
         * Check the given memory range that was previously painted to
         * ensure the painted values are still present
         *
         * @param startAddy The address to start checking the paint
         * @param numBytes The number of bytes to check
         */
        static bool checkPaint(int* startAddy, size_t numBytes);

        /**
         * Calculate the 16bit crc for the given memory range.
         *
         * @param startAddy The address to start calculating the crc
         * @param numBytes The number of bytes to calculate
         *
         * @return 16bit CRC for the memory range.
         */
        static uint16_t crc16(int* startAddy, size_t numBytes);

        /**
         * Calculate the 32bit crc for the given memory range.
         *
         * @param startAddy The address to start calculating the crc
         * @param numBytes The number of bytes to calculate
         *
         * @return 32bit CRC for the memory range.
         */
        static uint32_t crc32(int* startAddy, size_t numBytes);

    private:
        Memory() {}
        virtual ~Memory() {}

        Memory(Memory& copy) { (void)copy; };

        static const uint32_t PAINT_VALUE = 0xDEADBEEF;
};

#endif
