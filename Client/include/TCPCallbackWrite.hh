#ifndef TCP_CALLBACK_WRITE_HH_
# define TCP_CALLBACK_WRITE_HH_

# include <memory>

# include "ISocket.hpp"

namespace Network
{
  template <typename T>
  class ISocket;
  
  struct TCPCallbackWrite
  {
  private :
    using SocketPtr = std::shared_ptr<Network::ISocket<int>>;

  public :
    TCPCallbackWrite() = delete;
    ~TCPCallbackWrite() = delete;
    static void write(const SocketPtr&);
  };
}

#endif /* TCP_CALLBACK_WRITE_HH_ */
