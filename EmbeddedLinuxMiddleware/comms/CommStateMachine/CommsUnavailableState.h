#ifndef COMMSUNAVAILABLESTATE_H_
#define COMMSUNAVAILABLESTATE_H_

namespace CecilStLabs
{
   /**
    * State implementation for communication being unavailable
    */
   class CommsUnavailableState : public State, public ILoggable
   {
      public:

         /**
          * Create the "communications unavailable" state with the specified
          * data access layer, and HTTPS endpoint.
          *
          * @param commDAL data access layer containing the messages to send to the server.
          * @param https Secure HTTP communication method.
          */
         CommsUnavailableState(CommDAL* commDAL,
                               ICommProtocol* websocket,
                               ICommProtocol* https,
                               MessageParser& parser,
                               uint8_t reconnectInterval );

         /**
          * release any resources created by the state.
          */
         virtual ~CommsUnavailableState();

         /**
          * Perform the entry logic for this state.
          */
         void enter();

         /**
          * Perform the exit logic for this state.
          */
         void exit();

         /**
          * When communications unavailable get the "oldest" record from the
          * database and attempt to send it to the device web server.
          */
         void execute();

      protected:

         /**
          * Queue containing desired communications to the server.
          */
         CommDAL* m_commDAL;

         /**
          * protocol for posting messages to the server.
          */
         ICommProtocol* m_websocketProtocol;

         /**
          * protocol for posting messages to the server.
          */
         ICommProtocol* m_httpsProtocol;

         /**
          * message parser for responses to messages.
          */
         MessageParser& m_parser;

         /**
          * interval to wait before trying to reconnect
          */
         uint8_t m_reconnectInterval;

         /**
          * counter to use for reconnect interval
          */
         uint8_t m_reconnectCounter;
   };
}
#endif
