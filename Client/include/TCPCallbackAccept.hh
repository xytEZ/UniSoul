#ifndef TCP_CALLBACK_ACCEPT_HH_
# define TCP_CALLBACK_ACCEPT_HH_

# include "IMultiplexer.hh"

namespace Network
{
  struct TCPCallbackAccept
  {
  private :
    using SocketPtr = std::shared_ptr<Network::ISocket<int>>;
    using TCPSocketPtr = std::shared_ptr<Network::ITCPSocket<int>>;
    
    using TCPSocketServerPtr
    = std::shared_ptr<Network::ITCPSocketServer<int, TCPSocketPtr>>;

  public :
    TCPCallbackAccept() = delete;
    ~TCPCallbackAccept() = delete;    
    static void accept(const SocketPtr&, Network::IMultiplexer&);

  private :
    static void setEntryConnectionInfo(TCPSocketPtr&);
  };
}

#endif /* !TCP_CALLBACK_ACCEPT_HH_ */
