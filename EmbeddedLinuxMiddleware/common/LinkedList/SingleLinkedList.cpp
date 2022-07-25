include "../CriticalSection/CriticalSection.h"

#define EXT_SingleLinkedListCPP
#include "SingleLinkedList.h"

SingleLinkedList::SingleLinkedList ()
{
    pHead = NULL;
}

bool SingleLinkedList::insert (SingleLink *pInsertLink,
                               SingleLink *pAfterLink)
{
    bool inserted = false;
    SingleLink *pLocation;
    SingleLink *pCheck;

    m_criticalSection.Enter();

    // Valid link to insert?
    if (pInsertLink != NULL)
    {
        // Link questionable for already being in a list?
        if (pInsertLink->pNextLink == NULL)
        {
            // Ensure link not at end of this list, and find
            // if pAfterLink is in the list at the same time
            pCheck = pHead;
            pLocation = (pCheck == pAfterLink ? pCheck : NULL);

            if (pCheck != NULL)
            {
                while (pCheck->pNextLink != NULL)
                {
                    pCheck = pCheck->pNextLink;
                    if (pCheck == pAfterLink)
                    {
                        pLocation = pCheck;
                    }
                }
            }

            if (pCheck != pInsertLink)
            {
                // Insert in middle of list?
                if (pAfterLink != NULL)
                {
                    // is pAfterLink in the list?
                    if (pLocation == pAfterLink)
                    {
                        pInsertLink->pNextLink = pAfterLink->pNextLink;
                        pAfterLink->pNextLink = pInsertLink;
                        inserted = true;
                    }
                }

                // Else insert at head of list
                else
                {
                    pInsertLink->pNextLink = pHead;
                    pHead = pInsertLink;
                    inserted = true;
                }
            }
        }
    }

    m_criticalSection.Exit();

    return (inserted);
}

bool SingleLinkedList::addToEnd (SingleLink *pAddLink)
{
    bool added = false;

    if(NULL != pHead)
    {
       insert(pAddLink, last());
    }
    else
    {
       pHead = pAddLink;
    }

    return added;
}


SingleLink * SingleLinkedList::remove (SingleLink *pRemoveLink)
{
    SingleLink *pFound;
    SingleLink *pPrevious;

    m_criticalSection.Enter();

    pFound = pHead;
    pPrevious = NULL;

    // Find link to remove
    if (pRemoveLink != NULL)
    {
        while (pFound != NULL && pFound != pRemoveLink)
        {
            pPrevious = pFound;
            pFound = pFound->pNextLink;
        }

        if (pFound != NULL)
        {
            // The first link?
            if (pPrevious == NULL)
            {
                pHead = pHead->pNextLink;
            }
            else
            {
                pPrevious->pNextLink = pFound->pNextLink;
            }

            pFound->pNextLink = NULL;
        }
    }

    // Remove first link
    else if (pHead != NULL)
    {
        pHead = pHead->pNextLink;
        pFound->pNextLink = NULL;
    }

    else
    {
        // List is empty
    }

    m_criticalSection.Exit();

    return (pFound);
}

SingleLink * SingleLinkedList::removeFromEnd ()
{
   SingleLink* pRetLink = NULL;

   if( NULL != pHead )
   {
      pRetLink = remove(last());
   }

   return pRetLink;
}


SingleLink * SingleLinkedList::first (void)
{
    return (pHead);
}

SingleLink * SingleLinkedList::last (void)
{
    SingleLink* retLink = NULL;
    SingleLink* tmpLink;

    if(NULL != pHead)
    {
        retLink = pHead;

        tmpLink = next(pHead);
        while(NULL != tmpLink)
        {
            retLink = tmpLink;
            tmpLink = next(tmpLink);
        }
    }

    return retLink;
}

SingleLink* SingleLinkedList::next (SingleLink *pCurrent)
{
    m_criticalSection.Enter();

    if (pCurrent != NULL)
    {
        pCurrent = pCurrent->pNextLink;
    }
    else
    {
        pCurrent = pHead;
    }

    m_criticalSection.Exit();

    return (pCurrent);
}

bool SingleLinkedList::inList (SingleLink *pInList)
{
    SingleLink *pFound;

    m_criticalSection.Enter();

    pFound = pHead;

    while (pFound != NULL && pFound != pInList)
    {
        pFound = pFound->pNextLink;
    }

    m_criticalSection.Exit();

    return (pFound != NULL);
}

SingleLink* SingleLinkedList::find (sllUserFunc_t funcEqual,
                                    void *pData)
{
    SingleLink *pFound;
    bool equal = false;

    m_criticalSection.Enter();

    pFound = pHead;

    while (pFound != NULL && !equal)
    {
        if (funcEqual(pData, pFound))
        {
            equal = true;
        }
        else
        {
            pFound = pFound->pNextLink;
        }
    }

    m_criticalSection.Exit();

    return (pFound);
}

sllListLength_t SingleLinkedList::length (void)
{
    sllListLength_t length = 0;
    SingleLink *pTraverse;

    m_criticalSection.Enter();

    pTraverse = pHead;

    while (pTraverse != NULL)
    {
        ++length;
        pTraverse = pTraverse->pNextLink;
    }

    m_criticalSection.Exit();

    return (length);
}
