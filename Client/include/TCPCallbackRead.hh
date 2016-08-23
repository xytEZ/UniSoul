#ifndef TCP_CALLBACK_READ_HH_
# define TCP_CALLBACK_READ_HH_

# include "IMultiplexer.hh"

namespace Network
{  
  struct TCPCallbackRead
  {
  private :
    using SocketPtr = std::shared_ptr<Network::ISocket<int>>;

  public :
    TCPCallbackRead() = delete;
    ~TCPCallbackRead() = delete;
    static void read(const SocketPtr&, Network::IMultiplexer&);
  };
}

#endif /* !TCP_CALLBACK_READ_HH_ */
