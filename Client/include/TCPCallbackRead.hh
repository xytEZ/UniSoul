#ifndef TCP_CALLBACK_READ_HH_
# define TCP_CALLBACK_READ_HH_

# include <memory>

namespace Network
{
  template <typename T>
  class ISocket;
  
  struct TCPCallbackRead
  {
  private :
    using SocketPtr = std::shared_ptr<Network::ISocket<int>>;

  public :
    TCPCallbackRead() = delete;
    ~TCPCallbackRead() = delete;
    static void read(const SocketPtr&);
  };
}

#endif /* !TCP_CALLBACK_READ_HH_ */
