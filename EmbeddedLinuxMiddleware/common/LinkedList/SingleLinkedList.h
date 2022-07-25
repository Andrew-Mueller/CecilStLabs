/**
 * Maintains a list of SingleLink objects. The SingleLinkedList class
 * supports synchronous access to the list for inserting, removing and
 * traversing links in the list on a per operation basis, given the proper
 * user defined macros for interrupt and task locking. Synchronous operation
 * is NOT supported across operations. For example, traversing to a link, then
 * accessing data in that link. If links are to be accessed while they remain
 * in the list, the application software needs to provide protection for
 * synchronous operation.
 */

#ifndef __INCSingleLinkedListCPPh
#define __INCSingleLinkedListCPPh

#include "../basicTypes.h"                   //lint !e537
#include "LinkedListCommon.h"
#include "../CriticalSection/CriticalSection.h"

#ifndef EXT_SingleLinkedListCPP
#define EXT_SingleLinkedListCPP extern
#endif

class SingleLinkedList
{
   public:
      /**
       * Default constructor for the SingleLinkedList class
       */
      SingleLinkedList ();

      /**
       * Inserts a link after the specified link. Inserts a link at the head of
       * the list if NULL is specified. Returns true if the link was inserted.
       * Returns false if the <afterLink> does not exist in the list or the
       * <insertLink> is already in the list, or the <insertLink> next link
       * pointer is not NULL (an indication the link is in a different list).
       */
      bool insert (SingleLink *pInsertLink, SingleLink *pAfterLink = NULL);

      /**
       * Adds a link at the end of the list
       */
      bool addToEnd (SingleLink *pAddLink);

      /**
       * Removes the specified link from the list. Returns a pointer to the
       * removed link if the link was successfully removed. Returns NULL if the
       * link could not be removed because it could not be found in the list.
       * Specifying NULL for the link to be removed will remove the link at the
       * head of the list.
       */
      SingleLink* remove (SingleLink *pRemoveLink = NULL);

      /**
       * Remove the last link in the list
       */
      SingleLink* removeFromEnd();

      /**
       * Returns a pointer to the first link in the list. NULL if the list is
       * empty.
       */
      SingleLink* first (void);

      /**
       * Returns a pointer to the last link in the list. NULL if the list is
       * empty.
       */
      SingleLink* last (void);

      /**
       * Returns a pointer to the next link in the list past <pCurrent>.
       * NULL if <pCurrent> is the last link.
       */
      SingleLink* next (SingleLink *pCurrent);

      /**
       * Returns true if the specified link is in the list, False if not.
       */
      bool inList (SingleLink *pInList);

      /**
       * Find a link in the list based on the data in the link. The data is
       * compared using a user defined comparison function. Returns a pointer
       * to the first link found, otherwise NULL if no link was found.
       */
      SingleLink* find (sllUserFunc_t funcEqual, void *pData);

      /**
       * Returns the number of links in the list.
       */
      sllListLength_t length (void);

   protected:
      SingleLink *pHead;

      CecilStLabs::CriticalSection m_criticalSection;

   private:
      /**
       * Copy constructor for the SingleLinkedList class. It is not possible to
       * copy linked lists since the nature of the data the links point to is
       * unknown. So the copy constructor is defined and marked as private.
       */
      SingleLinkedList (const SingleLinkedList &lnkList);         //lint !e1704
      SingleLinkedList& operator= (SingleLinkedList& lnkList);    //lint !e1720
};

#endif
