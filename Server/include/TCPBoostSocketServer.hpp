#ifndef TCP_BOOST_SOCKET_SERVER_HPP_
# define TCP_BOOST_SOCKET_SERVER_HPP_

# include "TCPBoostSocket.hh"
# include "ITCPSocketServer.hpp"

namespace Network
{
  template <typename T>
  class TCPBoostSocketServer : public TCPBoostSocket,
			       public ITCPSocketServer<T>
  {
  private :
    boost::asio::io_service	_ios;
    
  public :
    TCPBoostSocketServer();
    virtual ~TCPBoostSocketServer();
    virtual bool open(int, int, int);
    virtual bool close();
    virtual bool send(const std::string&) const;
    virtual std::string recv() const;
    virtual bool bind(const t_sockaddr *, int);
    virtual bool listen(int);
    virtual T accept(t_sockaddr *, int *);
  };

  template <typename T>
  TCPBoostSocketServer<T>::TCPBoostSocketServer() : TCPBoostSocket(_ios) { }

  template <typename T>
  TCPBoostSocketServer<T>::~TCPBoostSocketServer() { }

  template <typename T>
  bool TCPBoostSocketServer<T>::open(int, int, int)
  {
    return TCPBoostSocket::open(0, 0, 0);
  }

  template <typename T>
  bool TCPBoostSocketServer<T>::close()
  {
    return TCPBoostSocket::close();
  }

  template <typename T>
  bool TCPBoostSocketServer<T>::send(const std::string&) const
  {
    return TCPBoostSocket::send("");
  }

  template <typename T>
  std::string TCPBoostSocketServer<T>::recv() const
  {
    return TCPBoostSocket::recv();;
  }
  
  template <typename T>
  bool TCPBoostSocketServer<T>::bind(const t_sockaddr *, int)
  {
    return true;
  }

  template <typename T>
  bool TCPBoostSocketServer<T>::listen(int) { return true; }

  template <typename T>
  T TCPBoostSocketServer<T>::accept(t_sockaddr *, int *) { return T(); }
}

#endif /* !TCP_BOOST_SOCKET_SERVER_HPP_ */
