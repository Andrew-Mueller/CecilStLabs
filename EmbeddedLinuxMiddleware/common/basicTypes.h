/*
 * basicTypes.h
 *
 * Description:
 * Definitions of strong types used as "basic types" with specific sizes for the
 * target hardware.
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <cstdint>
#include <string>

// pick up the standard defined types.
#ifndef NULL
   #define NULL   (0)
#endif

static const unsigned char MUTEX_SUCCESS = 0;

// forward definition of the semaphore type.  This allows not needing to
// add unnecessary header includes.
typedef int sem_t;

// File Descriptor for ioctl
typedef int file_desc_t;

/**
 * Constant definition for the number of bits in a byte on this platform.
 *
 * NOTE: When will a byte not be eight bits?  If we port the common code to a
 *       Digital Signal Processor (DSP) for example, they often do not have
 *       "byte addressable memory but word addressable instead".  In this case
 *       the smallest addressable unit might be 16 bits, or 24 bits.
 */
static const unsigned char NUM_BITS_IN_BYTE = 8;

/**
 * 8 bit wide unsigned     byte.
 */
typedef unsigned char byte_t;
#define CHECK_BYTE()    (sizeof(byte_t) == 1)
static const byte_t MIN_BYTE_VALUE = 0x00;
static const byte_t MAX_BYTE_VALUE = 0xFF;


/**
 * unsigned integer that is 8 bits wide.
 */
//typedef unsigned char uint8_t;                              //lint !e761
#define CHECK_UINT8()   (sizeof(uint8_t) == 1)
//static const uint8_t MIN_UINT8_VALUE = 0;
//static const uint8_t MAX_UINT8_VALUE = 255;


/**
 * signed integer that is 8 bits wide.
 */
//typedef signed char int8_t;                                 //lint !e761 !e1784
#define CHECK_INT8()    (sizeof(int8_t) == 1)
//static const int8_t MIN_INT8_VALUE = -128;
//static const int8_t MAX_INT8_VALUE =  127;

/**
 * unsigned integer that is 16 bits wide.
 */
//typedef unsigned short uint16_t;                            //lint !e761
#define CHECK_UINT16()  (sizeof(uint16_t) == 2)
//static const uint16_t MIN_UINT16_VALUE = 0;
//static const uint16_t MAX_UINT16_VALUE = 65535;

/**
 * signed integer that is 16 bits wide.
 */
//typedef signed short int16_t;                               //lint !e761 !e1784
#define CHECK_INT16()   (sizeof(int16_t) == 2)
//static const int16_t MIN_INT16_VALUE = -32678;
//static const int16_t MAX_INT16_VALUE =  32767;

/**
 * unsigned integer that is 32 bits wide.
 */
//typedef unsigned int uint32_t;                              //lint !e761
#define CHECK_UINT32()  (sizeof(uint32_t) == 4)
//static const uint32_t MIN_UINT32_VALUE = 0;
//static const uint32_t MAX_UINT32_VALUE = 4294967294u;

/**
 * signed integer that is 32 bits wide.
 */
//typedef signed int int32_t;                                 //lint !e761 !e1784
#define CHECK_INT32() (sizeof(int32_t) == 4)
//static const int32_t MIN_INT32_VALUE = -2147483647;
//static const int32_t MAX_INT32_VALUE =  2147483647;

/**
 * unsigned integer that is 64 bits wide.
 */
//typedef unsigned long long uint64_t;                         //lint !e761 !e1784
#define CHECK_UINT64() (sizeof(uint64_t) == 8)
//static const uint64_t MIN_UINT64_VALUE = 0;
//static const uint64_t MAX_UINT64_VALUE = 18446744073709551615ull;

/**
 * signed integer that is 64 bits wide.
 */
//typedef long long int64_t;                                   //lint !e761 !e1784
#define CHECK_INT64() (sizeof(int64_t) == 8)
//static const int64_t MIN_INT64_VALUE = -9223372036854775807ll;
//static const int64_t MAX_INT64_VALUE =  9223372036854775807ll;


#define CHECK_BASIC_TYPES() (CHECK_BYTE() && CHECK_UINT8() && CHECK_INT8() && CHECK_UINT16() && CHECK_INT16() && CHECK_UINT32() && CHECK_INT32() && CHECK_UINT64() && CHECK_INT64())

/**
 * Structure of the IP address.
 */
typedef struct tag_ipAddy
{
    uint8_t first_octet;
    uint8_t second_octet;
    uint8_t third_octet;
    uint8_t fourth_octet;
} ip_address_t;

static const ip_address_t MIN_IP_ADDY_VALUE = {  0,   0,   0,   0};
static const ip_address_t MAX_IP_ADDY_VALUE = {255, 255, 255, 255};

/**
 * create a strong type for the bsd socket to communicate over TCP.
 * This is the raw socket used for byte communication across the wire.
 */
typedef int socket_t;

/**
 * Enumeration containing the possible HTTP request methods.
 */
enum HTTP_Method_e
{
   GET  = 0,
   POST = 1
};

#endif
