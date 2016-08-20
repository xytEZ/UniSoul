#ifndef SERVER_MESSAGE_HH_
# define SERVER_MESSAGE_HH_

namespace Network
{
  struct ServerMessage
  {
    static constexpr const char *ERROR =
      "An error is occurred. Try again, please";
    
    static constexpr const char *ACCEPTED_CONNECTION =
      "Accepted connection. Welcome to the server";
    
    static constexpr const char *REFUSED_CONNECTION =
      "Refused connection. Try again, please";
    
    static constexpr const char *DISCONNECTION =
      "Goodbye and see you soon";
    
    ServerMessage() = delete;
    ~ServerMessage() = delete;
  };
}

#endif /* !SERVER_MESSAGE_HH_ */
