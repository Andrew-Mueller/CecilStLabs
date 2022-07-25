#ifndef COMMSSIGNALS_H_
#define COMMSSIGNALS_H_

namespace CecilStLabs
{
   /**
    * Signal enumeration
    */
   enum eCommsSignal
   {
      /**
       * The response from the server was good.
       */
      SIGNAL_HTTP_200         = 0,

      /**
       * The response from the server was not good.
       */
      SIGNAL_NON_HTTP_200     = 1,

      /**
       * Message has attempted to be sent.
       */
      SIGNAL_SEND_ATTEMPT     = 2,

      /**
       * Web Socket was closed.
       */
      SIGNAL_WEBSOCKET_CLOSED = 3,

      /**
       * Try web socket connection
       */
      SIGNAL_WEBSOCKET_OPEN = 4,

      /**
       * Do one HTTP request
       */
      SIGNAL_ONE_HTTP = 5
   };
}

#endif
