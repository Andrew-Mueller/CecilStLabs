//*****************************************************************************
//
// fontcmss16i.c - Font definition for the 16pt Cmss italic font.
//
// Copyright (c) 2011-2015 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.1.71 of the Tiva Graphics Library.
//
//*****************************************************************************

//*****************************************************************************
//
// This file is generated by ftrasterize; DO NOT EDIT BY HAND!
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "grlib/grlib.h"

//*****************************************************************************
//
// Details of this font:
//     Characters: 32 to 126 inclusive
//     Style: cmss
//     Size: 16 point
//     Bold: no
//     Italic: yes
//     Memory usage: 1992 bytes
//
//*****************************************************************************

//*****************************************************************************
//
// The compressed data for the 16 point Cmss italic font.
// Contains characters 32 to 126 inclusive.
//
//*****************************************************************************
static const uint8_t g_pui8Cmss16iData[1789] =
{
      5,   6,   0,  12,  96,  13,   5, 194,  50,  49,  65,  65,
     50,  49,  65, 210, 240, 128,  11,   6, 194,  18,  33,  33,
     17,  33,   0,   9,  32,  29,  12, 240, 241, 177,  33, 113,
     33, 129,  33, 113,  33,  90,  65,  49, 113,  33,  90,  65,
     33, 129,  33, 113,  33, 129,  33, 177, 240,  64,  23,   7,
    162,  68,  35,  17,  33,  17,  65,  17,  67,  83,  65,  17,
     65,  17,  65,  17,  66,  52, 240, 240,  16,  40,  10, 194,
     65,  33,  17,  49,  33,  33,  49,  33,  33,  33,  49,  17,
     33,  67,  17, 145,  34,  65,  33,  17,  49,  33,  33,  49,
     33,  33,  33,  49,  33,  17,  65,  17,  33,  66, 240, 240,
     48,  28,  11, 240, 179, 129,  17, 113,  33, 113,  17, 130,
    145,  66,  50,  65,  49,  33,  33,  49,  49,  17,  65,  50,
     99,  35,   0,   5,  96,   8,   3,  98,  33,  17, 240, 240,
    128,  20,   6,  65,  65,  81,  65,  81,  65,  81,  81,  65,
     81,  81,  81,  81,  81,  81,  81,  97,  64,  20,   6,  49,
     81,  97,  81,  81,  81,  81,  81,  81,  65,  81,  81,  65,
     81,  65,  81,  65,  80,  16,   7,  49,  97,  65,  17,  17,
     36,  66,  51,  17,  65,  97,   0,   8,  48,  16,  11,   0,
      6,  17, 161, 161, 145, 106,  81, 161, 161, 161,   0,   6,
     32,   8,   3, 240, 240,  98,  17,  33, 128,   8,   5, 240,
    240, 244, 240, 240,  96,   5,   2, 240, 145, 144,  20,  10,
    129, 129, 145, 129, 145, 129, 145, 129, 145, 129, 145, 129,
    145, 129, 145, 129, 145, 144,  26,   8, 240,  67,  65,  49,
     33,  65,  33,  65,  33,  65,  17,  81,  17,  65,  33,  65,
     33,  65,  33,  49,  67, 240, 240,  96,  16,   6, 240,  17,
     51,  66,  65,  81,  81,  81,  66,  65,  81,  53, 240, 160,
     19,   8, 240,  67,  65,  49,  33,  65, 113, 113,  97,  97,
     97,  82,  81, 118, 240, 240,  64,  19,   8, 240,  67,  65,
     34, 113,  97, 113,  67, 129, 113, 113,  33,  49,  67, 240,
    240,  96,  21,   8, 240,  98,  83,  82,  81,  17,  65,  33,
     50,  33,  34,  34,  39,  81, 113, 113, 240, 240,  80,  20,
      8, 240,  53,  49, 113,  98,  97,  18,  66,  33, 113, 113,
    113,  33,  49,  52, 240, 240,  96,  23,   8, 240,  83,  50,
     82,  97, 113,  18,  51,  33,  33,  65,  33,  65,  33,  50,
     33,  49,  67, 240, 240,  96,  17,   8, 240,  23,  97,  98,
     97,  97,  97, 113,  97, 113,  98,  97, 240, 240, 144,  25,
      8, 240,  67,  65,  49,  33,  65,  33,  65,  34,  33,  67,
     65,  49,  33,  65,  33,  65,  33,  49,  67, 240, 240,  96,
     23,   8, 240,  67,  65,  49,  49,  49,  33,  65,  33,  65,
     33,  65,  33,  50,  52, 113,  97,  52, 240, 240,  96,   8,
      4, 240, 162, 240,  98, 240,  48,   9,   4, 240, 177, 240,
     98,  33,  49, 176,  14,   5, 240, 194, 129,  65,  65,  50,
     49,  65,  65,  50,  50, 128,  10,  12,   0,  10,  90, 240,
    170,   0,   9,  32,  16,   6, 240, 240,  50, 161,  81,  65,
     81,  65,  65,  81,  81,  49,  20, 128,  15,   6, 211,  33,
     49,  81,  65,  65,  65,  81,  81, 240,  17, 240, 224,  31,
     10, 240, 148,  81,  50,  49,  36,  33,  33,  34,  17,  33,
     50,  17,  33,  49,  33,  33,  49,  33,  33,  34,  33,  50,
     81,  81,  69, 240, 240, 208,  25,   9, 240, 130, 114,  97,
     17,  97,  18,  65,  34,  65,  34,  49,  65,  39,  33,  81,
     18,  81,  17,  97, 240, 240, 112,  25,  10, 240, 118,  65,
     65,  65,  81,  34,  65,  50,  50,  53,  81,  66,  49,  81,
     34,  81,  33,  81,  54, 240, 240, 224,  18,   9, 240, 116,
     50, 113, 113, 113, 129, 129, 129, 129, 145,  65,  68, 240,
    240, 144,  26,  10, 240, 117,  66,  65,  50,  81,  33,  97,
     33,  97,  33,  97,  18,  97,  17,  97,  33,  82,  33,  66,
     53, 240, 240, 240,  17,   9, 240,  86,  34, 114, 113, 129,
    134,  34, 114, 113, 129, 134, 240, 240, 144,  17,   9, 240,
     86,  34, 114, 113, 129, 133,  50, 114, 113, 129, 129, 240,
    240, 224,  21,   9, 240, 116,  50,  98, 113, 113, 129, 129,
     52,  17,  81,  33,  81,  49,  65,  68, 240, 240, 144,  27,
     10, 240, 113,  81,  34,  81,  34,  81,  33,  82,  33,  82,
     39,  34,  81,  34,  81,  33,  82,  33,  82,  33,  81, 240,
    240, 208,  15,   5, 194,  50,  49,  65,  65,  50,  49,  65,
     65,  50,  50, 240, 128,  17,   8, 240, 113, 113,  98,  97,
    113, 113,  98,  98,  97,  98,  52, 240, 240,  96,  27,  10,
    240, 113,  81,  34,  65,  50,  49,  65,  34,  81,  18, 100,
     83,  18,  66,  49,  65,  66,  49,  81,  49,  82, 240, 240,
    192,  16,   7, 240,  17,  82,  82,  81,  97,  97,  82,  82,
     81,  97, 102, 240, 224,  41,  13, 240, 210,  98,  35,  83,
     35,  82,  49,  17,  65,  17,  49,  17,  65,  17,  49,  17,
     49,  33,  34,  18,  33,  18,  33,  49,  17,  33,  49,  51,
     33,  49,  50,  49,  49,  49,  65,   0,   6, 112,  34,  10,
    240, 114,  65,  35,  65,  33,  17,  65,  33,  18,  49,  33,
     33,  34,  33,  33,  33,  34,  33,  33,  33,  50,  17,  33,
     67,  33,  67,  33,  66, 240, 240, 208,  26,  10, 240, 147,
     82,  49,  65,  81,  33,  97,  17, 113,  17, 113,  17,  98,
     17,  97,  33,  82,  49,  50,  83, 240, 240, 240,  20,  10,
    240, 118,  66,  65,  49,  81,  49,  81,  49,  65,  54,  65,
    145, 145, 145, 130,   0,   6,  28,  10, 240, 147,  82,  49,
     65,  81,  33,  97,  17, 113,  17, 113,  17, 113,  17,  97,
     33,  49,  18,  49,  35,  84, 145, 146, 240, 128,  26,  10,
    240, 118,  65,  66,  49,  81,  49,  81,  34,  65,  54,  65,
     49,  81,  50,  65,  65,  50,  65,  49,  82, 240, 240, 192,
     19,   8, 240,  68,  49,  49,  33, 113, 114, 115, 129, 113,
    113,  33,  49,  52, 240, 240,  96,  17,  10, 240,  89,  66,
    130, 129, 145, 145, 130, 130, 129, 145, 145,   0,   5, 112,
     27,  10, 240,  98,  66,  33,  81,  49,  81,  49,  81,  49,
     81,  33,  82,  33,  81,  49,  81,  49,  66,  50,  34,  84,
    240, 240, 240,  26,  10, 240,  81, 113,  17,  97,  33,  82,
     34,  65,  50,  49,  66,  49,  81,  33,  97,  33,  97,  17,
    115, 114,   0,   5, 112,  41,  14, 240, 209,  81,  81,  17,
     66,  66,  17,  66,  65,  33,  49,  17,  50,  33,  49,  17,
     49,  49,  33,  33,  34,  49,  33,  34,  17,  65,  17,  52,
     65,  17,  51,  82,  67,  82,  66,   0,   7,  96,  25,  11,
    240, 146,  81,  65,  65,  82,  33, 113,  17, 130, 146, 131,
    114,  33,  82,  49,  81,  66,  49,  98,   0,   5,  96,  22,
     10, 240,  81,  98,  18,  81,  34,  65,  65,  49,  82,  17,
    115, 114, 129, 145, 145, 130,   0,   5, 112,  17,  11, 240,
    152, 130, 145, 145, 145, 146, 130, 130, 145, 145, 152,   0,
      5, 112,  19,   6, 146,  65,  81,  65,  81,  81,  81,  66,
     65,  81,  81,  81,  66,  65,  81,  82,  64,  11,   6, 209,
     33,  18,  18,  18,  18,   0,   9,  16,  19,   6, 146,  81,
     81,  81,  65,  81,  81,  81,  66,  65,  81,  81,  81,  66,
     65,  66,  64,   8,   5, 178,  33,  18,   0,   8,  32,   5,
      2,  65, 240, 224,   8,   3, 113,  18,  18, 240, 240, 112,
     16,   7, 240, 240, 227,  49,  49,  97,  52,  18,  34,  17,
     49,  37, 240, 240,  22,   8, 240,  65,  98,  97, 113, 116,
     65,  49,  34,  49,  33,  65,  33,  65,  33,  49,  37, 240,
    240,  80,  13,   7, 240, 240, 228,  33,  81,  97,  97,  97,
    116, 240, 240,  22,   8, 240, 113, 113, 113, 113,  53,  33,
     49,  33,  65,  33,  65,  33,  50,  33,  49,  68, 240, 240,
     80,  14,   7, 240, 240, 227,  49,  49,  22,  17,  97,  98,
    100, 240, 240,  17,   7, 240,  35,  49,  97,  97,  68,  65,
     97,  97,  97,  81,  97, 240, 240,  64,  21,  10,   0,   7,
    115,  18,  50,  33,  81,  49,  81,  34,  69,  81, 165,  50,
     65,  49,  66,  68, 240,  23,   7, 240,  17,  97,  97,  82,
     81,  19,  34,  33,  33,  49,  33,  49,  18,  34,  17,  49,
     33,  49, 240, 240,  12,   4, 161, 225,  49,  49,  49,  33,
     49,  49, 240,  64,  16,   8, 240,  98, 240, 241, 113,  98,
     97, 113, 113,  98,  97, 113,  67, 208,  23,   8, 240,  50,
     97, 113, 113, 113,  34,  34,  17,  65,  17,  83,  81,  33,
     65,  33,  49,  50, 240, 240,  64,  15,   5, 209,  50,  49,
     65,  65,  50,  49,  65,  65,  50,  49, 240, 144,  28,  11,
      0,   8,  49,  19,  19,  34,  34,  33,  33,  49,  49,  18,
     49,  49,  17,  50,  34,  17,  49,  49,  33,  49,  49,   0,
      5,  96,  20,   7, 240, 240, 209,  19,  34,  33,  33,  49,
     33,  49,  18,  34,  17,  49,  33,  49, 240, 240,  19,   8,
      0,   6,  36,  49,  65,  17,  81,  17,  81,  17,  66,  17,
     50,  52, 240, 240,  80,  21,   8,   0,   6,  33,  19,  50,
     33,  34,  49,  33,  65,  33,  65,  33,  49,  52,  49, 113,
    113, 240,  20,   8,   0,   6,  37,  33,  49,  33,  65,  33,
     65,  33,  50,  33,  49,  68, 113, 113,  98, 176,  14,   6,
    240, 240, 129,  17,  35,  50,  65,  81,  81,  65, 240, 224,
     15,   7, 240, 240, 228,  33,  97, 115,  97,  33,  49,  36,
    240, 240,  16,  15,   5, 240,  17,  65,  65,  52,  33,  65,
     49,  65,  65,  67, 240, 112,  20,   7, 240, 240, 209,  49,
     33,  49,  33,  34,  17,  50,  17,  49,  33,  34,  35,  17,
    240, 240,  18,   7, 240, 240, 193,  65,  17,  49,  33,  34,
     33,  33,  52,  66,  81, 240, 240,  48,  29,  10,   0,   7,
     65,  49,  49,  17,  34,  49,  17,  34,  33,  33,  17,  17,
     33,  33,  17,  17,  17,  50,  33,  17,  50,  49, 240, 240,
    224,  18,   9,   0,   7,   1,  50,  50,  18,  83,  98,  99,
     82,  33,  50,  50, 240, 240, 128,  20,   9,   0,   7,   1,
     65,  49,  49,  65,  34,  66,  17,  98, 114, 113, 129, 113,
     98, 240,  16,  13,   7, 240, 240, 213,  82,  66,  66,  66,
     66,  85, 240, 240,   8,   9,   0,   9,   8,   0,   9,  16,
      8,  16,   0,  16,  15,   0,  16,  16,   8,   6, 209,  18,
     17,  33,   0,  10,   9,   6, 210,  17,  17,  34,   0,   9,
    112,
};

//*****************************************************************************
//
// The font definition for the 16 point Cmss italic font.
//
//*****************************************************************************
const tFont g_sFontCmss16i =
{
    //
    // The format of the font.
    //
    FONT_FMT_PIXEL_RLE,

    //
    // The maximum width of the font.
    //
    14,

    //
    // The height of the font.
    //
    17,

    //
    // The baseline of the font.
    //
    13,

    //
    // The offset to each character in the font.
    //
    {
           0,    5,   18,   29,   58,   81,  121,  149,
         157,  177,  197,  213,  229,  237,  245,  250,
         270,  296,  312,  331,  350,  371,  391,  414,
         431,  456,  479,  487,  496,  510,  520,  536,
         551,  582,  607,  632,  650,  676,  693,  710,
         731,  758,  773,  790,  817,  833,  874,  908,
         934,  954,  982, 1008, 1027, 1044, 1071, 1097,
        1138, 1163, 1185, 1202, 1221, 1232, 1251, 1259,
        1264, 1272, 1288, 1310, 1323, 1345, 1359, 1376,
        1397, 1420, 1432, 1448, 1471, 1486, 1514, 1534,
        1553, 1574, 1594, 1608, 1623, 1638, 1658, 1676,
        1705, 1723, 1743, 1756, 1764, 1772, 1780,
    },

    //
    // A pointer to the actual font data
    //
    g_pui8Cmss16iData
};
