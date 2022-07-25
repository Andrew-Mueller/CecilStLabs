#ifndef TRANSITIONHANDLER_H_
#define TRANSITIONHANDLER_H_

namespace CecilStLabs
{

   // forward declare the IState interface to prevent circular inclusion
   class IState;

   /**
    * Interface for a handler of transitions.  Typically this will be the state
    * machine instance proper, but other objects may implement this as well.
    */
   class ITransitionHandler
   {
      public:
         virtual void handle(IState* newState) = 0;

      protected:

         /**
          * Specify the interfaces constructor, copy constructor, destructor and
          * assignment operator as non-public to ensure that an instance of this
          * interface cannot be created.
          */
         ITransitionHandler() {};
         ITransitionHandler(ITransitionHandler* copy __attribute__((unused))) {};
         virtual ~ITransitionHandler() {};
         ITransitionHandler* operator=(const ITransitionHandler* copy __attribute((unused))) { return NULL; };
   };

}
#endif
