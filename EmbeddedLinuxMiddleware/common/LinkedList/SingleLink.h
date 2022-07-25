/**
 * Maintains the information for a link in a single linked list. SingleLink(s)
 * should be managed by a SingleLinkedList class. The SingleLink class does
 * not provide any sort of data access protection, the application software is
 * responsible for providing any necessary protection (e.g. semaphores).
 *
 * If desired a wrapper class may be derived from SingleLink which provides
 * for synchronous protection. This wrapper class would probably rely on
 * semaphores provided by an operating system in a multi-tasking environment,
 * or it would rely on simply interrupt locks in a single threaded environment
 * with interrupts. By deriving a wrapper class, the SingleLinkedList manager
 * will still be able to manager the link objects using the SingleLink
 * interface, while the application uses the wrapper class interface to access
 * the data.
 *
 * In order to put actual data into a list, a class needs to be derived from
 * SingleLink which then contains the data.
 */


#ifndef __INCSingleLinkCPPh
#define __INCSingleLinkCPPh

#include "../basicTypes.h"                   //lint !e537

#ifndef EXT_SingleLink
#define EXT_SingleLink extern
#endif

class SingleLink
{
   public:
      /**
       * Default constructor for the SingleLink class.
       */
      SingleLink ();

      /**
       * Pointer to the next link in a list. Should ONLY be
       * referenced by a linked list manager.
       */
      SingleLink *pNextLink;

   protected:
      /**
       * Copy constructor for the SingleLink class. It is not possible to copy
       * links since the nature of the data the link points to is unknown.
       * Therefore the copy constructor is defined and marked as private.
       */
      SingleLink (const SingleLink &slink __attribute__((unused))) : pNextLink(NULL) {};        //lint !e1704
      SingleLink& operator= (SingleLink& slink) { return slink; };   //lint !e1720
};

#endif
