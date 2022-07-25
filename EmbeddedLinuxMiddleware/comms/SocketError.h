#ifndef SOCKETERROR_H_
#define SOCKETERROR_H_

namespace CecilStLabs
{
   class SocketError
   {
      public:
         static const uint8_t connection_success               = 0;
         static const uint8_t error_creating_socket            = 1;
         static const uint8_t error_init_ip_address            = 2;
         static const uint8_t error_failed_setting_rx_timeout  = 3;
         static const uint8_t error_connecting_socket          = 4;
         static const uint8_t error_sending_header             = 5;
         static const uint8_t error_parsing_header_response    = 6;

         // specific secure socket errors
         static const uint8_t error_creating_ssl_context       = 7;
         static const uint8_t error_connecting_ssl             = 8;

         SocketError(uint8_t error) : m_error(error) { };
         SocketError(const SocketError& error) { this->m_error = error.m_error; };
         virtual ~SocketError() { };


         // getter and setter for the internal error.
         uint8_t getError() { return m_error; };
         void setError(uint8_t error) { m_error = error; };

      protected:

         /**
         * The internal error held by the socket error.
         */
         uint8_t m_error;
   };
}
#endif
