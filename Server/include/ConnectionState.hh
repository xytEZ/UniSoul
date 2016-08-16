#ifndef CONNECTION_STATE_HH_
# define CONNECTION_STATE_HH_

namespace Network
{
  enum class ConnectionState
  {
    ACCEPTED_CONNECTION,
      REFUSED_CONNECTION,
      DISCONNECTION,
      NONE
      };
}

#endif /* !CONNECTION_STATE_HH_ */
