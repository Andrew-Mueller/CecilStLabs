#ifndef BIDIRECTIONALCOMMSAVAILABLESTATE_H_
#define BIDIRECTIONALCOMMSAVAILABLESTATE_H_

namespace CecilStLabs
{

   /**
    * State within the Communication's state machine responsible for the
    * bi-directional communication with the server (websockets for instance).
    */
   class BidirectionalCommsAvailableState : public State, public ILoggable
   {
      public:
         /**
          * Create the state using the specified communications queue and the
          * communication method to use to send the messages.
          *
          * @param commQueue queue containing the messages to send to the server.
          *
          * @param protocol The communication protocol method used to send messages to the server.
          *
          * @param parser The parser of the responses that are returned from the server.
          */
         BidirectionalCommsAvailableState(CommDAL* commDAL,
                                          ICommProtocol* protocol,
                                          MessageParser& parser);

         /**
          * release any resources used by the state.
          */
         virtual ~BidirectionalCommsAvailableState();

         /**
          * enter the bidirection comm available state
          */
         void enter();

         /**
          * perform the state's action
          */
         void execute();

         /**
          * actions to perform when exiting the state.
          */
         void exit();

      protected:

         /**
          * Internal function to organize code... sends a record out websocket
          */
         bool sendWebsocketRecord( CommRecord* record );

         /**
          * Queue containing desired communications to the server.
          */
         CommDAL* m_commDAL;

         /**
          * protocol for posting messages to the server.
          */
         ICommProtocol* m_protocol;

         /**
          * message parser for responses to messages.
          */
         MessageParser& m_parser;
   };

}
#endif
