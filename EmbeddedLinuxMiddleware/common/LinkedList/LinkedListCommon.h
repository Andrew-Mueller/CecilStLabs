#ifndef __INCLinkedListCommonCPPh
#define __INCLinkedListCommonCPPh

#include "SingleLink.h"

#ifndef EXT_SingleLinkedListCPP
#define EXT_SingleLinkedListCPP extern
#endif

typedef uint32_t sllListLength_t;
typedef bool (* sllUserFunc_t)(void *pDataSrc1, SingleLink *pLink);

#endif
