#ifndef SERVER_MESSAGE_HH_
# define SERVER_MESSAGE_HH_

namespace Network
{
  struct ServerMessage
  {
    static constexpr const char *ERROR =
      "An error is occurred. Try again, please";
    
    static constexpr const char *REFUSED_CONNECTION =
      "Refused connection. Try again, please";
    
    ServerMessage() = delete;
    ~ServerMessage() = delete;
  };
}

#endif /* !SERVER_MESSAGE_HH_ */
