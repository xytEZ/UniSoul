#ifndef SERVER_MESSAGE_HH_
# define SERVER_MESSAGE_HH_

namespace Network
{
  struct ServerMessage
  {
    static const char *ERROR;
    static const char *ACCEPTED_CONNECTION;
    static const char *REFUSED_CONNECTION;
    static const char *DISCONNECTION;

    ServerMessage() = delete;
    ~ServerMessage() = delete;
  };
}

#endif /* !SERVER_MESSAGE_HH_ */
