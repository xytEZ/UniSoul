#ifndef TCP_CALLBACK_WRITE_HH_
# define TCP_CALLBACK_WRITE_HH_

# include "IMultiplexer.hh"

namespace Network
{  
  struct TCPCallbackWrite
  {
  private :
    using SocketPtr = std::shared_ptr<Network::ISocket<int>>;

  public :
    TCPCallbackWrite() = delete;
    ~TCPCallbackWrite() = delete;
    static void write(const SocketPtr&, Network::IMultiplexer&);
  };
}

#endif /* TCP_CALLBACK_WRITE_HH_ */
