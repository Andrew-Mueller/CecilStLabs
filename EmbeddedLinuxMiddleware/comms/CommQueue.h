#ifndef COMMQUEUE_H_
#define COMMQUEUE_H_

namespace CecilStLabs
{
   /**
    * The CommQueue is responsible for maintaining data intended to be sent to
    * the server if the server could not be communicated with.
    */
   class CommQueue
   {
      public:

         /**
          * Create the communication queue with the specified socket.
          *
          * @param queueFileName the name of the persistent storage of queued
          *                      data on disk.
          * @param endpointURL the url of the device endpoint.
          * @param parser Parses the message responses when they are dequeued and
          *               processed.
          * @param https the https protocol to use
          * @param websocketprot the websocket protocol to use
          * @param logDriver Driver used to log messages.
          */
         CommQueue(std::string queueFileName,
                   std::string endpointURL,
                   messageParser& parser,
                   ICommProtocol* https,
                   ICommProtocol* websocketprot,
                   uint8_t commsReconnectInterval,
                   ILogDriver& logDriver);

         /**
          * release the resources setup by this object.
          */
         virtual ~CommQueue();

         /**
          * Adds the specified communication record to the queue of outgoing
          * communications.
          *
          * @param newRecord Communication record based on the data store
          */
         void enqueue(CommRecord& newRecord);

         /**
          * Gets the message from the communication queue.
          *
          * @return Communication record retrieved from the queue or NULL if
          *                       none were available.
          */
         CommRecord* get();

         /**
          * Removes the specified record from the communication queue.
          *
          * @param record Communication record to remove from the queue.
          */
         void remove(CommRecord& record);

         /**
          * Execute's the current state in the state machine.
          */
         void execute();

         /**
          * get the endpoint URL from the Comm Queue
          * `
          * @return string representing the URL of the server
          */
         inline std::string getEndpointURL() { return m_httpsProtocol->getHostname(); };

      protected:

         /**
          * The persistent backing store where data is put when communications
          * with the server cannot be performed.
          */
         CommDAL m_commDAL;

         /**
          * The openSSL secure session used for all secure communications to the server.
          */
         SecureSession m_session;

         /**
          * pointer to the openSSL over http communication protocol.
          */
         ICommProtocol* m_httpsProtocol;

         /**
          * pointer to the websockets bi-directional communication protocol.
          */
         ICommProtocol* m_websocketProtocol;

         /**
          * The state machine that contains all states and their associated
          * transitions for communication
          */
         StateMachine m_commStateMachine;

         /**
          * State for behaviors when bi-directional communications are available.
          */
         BidirectionalCommsAvailableState m_biCommsAvailableState;

         /**
          * State for behaviors for when the communications are ready and available.
          * This one will empty all queued messages before returning back to WS
          */
         UnidirectionalCommsAvailableState m_uniCommsAvailableState;

         /**
          * State for behaviors for when the communications are ready and available.
          * This one will only send one message before returning back to WS
          */
         UnidirectionalCommsAvailableState m_uniSingleCommsAvailableState;

         /**
          * State for behaviors for when the communications are not available.
          */
         CommsUnavailableState m_commsUnavailableState;

         /**
          * transitions from available state to unavailable state (and vice versa).
          */
         Transition m_transHTTP200;
         Transition m_transHTTP_NON_HTTP200;
         Transition m_transWS_CLOSED;
         Transition m_transUnavailableWS_OPEN;
         Transition m_transUnidirectionalWS_OPEN;
         Transition m_transWS_Single;
         Transition m_transitionSingle_NON;
         Transition m_transitionSingle_WS;
   };

}
#endif
