#ifndef TCP_BOOST_SOCKET_SERVER_HPP_
# define TCP_BOOST_SOCKET_SERVER_HPP_

# include <utility>
# include <boost/bind.hpp>
# include "TCPBoostSocket.hpp"
# include "ITCPSocketServer.hpp"

namespace Network
{
  template <int N, int N2, typename T = TCPBoostSocket<N, N2>>
    class TCPBoostSocketServer : public TCPBoostSocket<N, N2>,
				 public ITCPSocketServer<T>
  {
  private :
    template <int M, int M2, typename U>
    struct HandlerAsyncAccept
    {
      static void handleAccept(TCPBoostSocketServer<M, M2, U>&,
			       std::shared_ptr<TCPBoostSocket<M, M2>>&,
			       const boost::system::error_code&);
    };
    
  private :
    boost::asio::ip::tcp::acceptor	_acceptor;
    boost::asio::ip::tcp::endpoint	_endpoint;
    
  public :
    TCPBoostSocketServer(boost::asio::io_service&,
			 typename TCPBoostSocket<N, N2>::ComplexSystem&,
			 const std::string&,
			 int);
    virtual ~TCPBoostSocketServer();
    virtual bool open(int, int, int);
    virtual bool close();
    virtual bool send(const std::string&);
    virtual std::string recv();
    virtual bool bind(const t_sockaddr *, int);
    virtual bool listen(int);
    virtual T accept(t_sockaddr *, int *);

  private :
    template <typename HandlerPolicy>
    void handleAccept(std::shared_ptr<TCPBoostSocket<N, N2>>&,
		      const boost::system::error_code&);
  };

  template <int N, int N2, typename T>
  TCPBoostSocketServer<N, N2, T>::TCPBoostSocketServer(boost::asio::io_service& ios,
						       typename TCPBoostSocket<N, N2>::ComplexSystem& complexSystem,
						       const std::string& hostname,
						       int port) :
    TCPBoostSocket<N, N2>(ios, complexSystem),
    _acceptor(ios),
    _endpoint(boost::asio::ip::address::from_string(std::move(hostname)), port)
  {
  }

  template <int N, int N2, typename T>
  TCPBoostSocketServer<N, N2, T>::~TCPBoostSocketServer() { }

  template <int N, int N2, typename T>
  bool TCPBoostSocketServer<N, N2, T>::open(int, int, int)
  {
    _acceptor.open(_endpoint.protocol());
    _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    return true;
  }

  template <int N, int N2, typename T>
  bool TCPBoostSocketServer<N, N2, T>::close()
  {
    return TCPBoostSocket<N, N2>::close();
  }

  template <int N, int N2, typename T>
  bool TCPBoostSocketServer<N, N2, T>::send(const std::string& msg)
  {
    return TCPBoostSocket<N, N2>::send(msg);
  }

  template <int N, int N2, typename T>
  std::string TCPBoostSocketServer<N, N2, T>::recv()
  {
    return TCPBoostSocket<N, N2>::recv();
  }
  
  template <int N, int N2, typename T>
  bool TCPBoostSocketServer<N, N2, T>::bind(const t_sockaddr *, int)
  {
    _acceptor.bind(_endpoint);
    return true;
  }

  template <int N, int N2, typename T>
  bool TCPBoostSocketServer<N, N2, T>::listen(int)
  {
    _acceptor.listen();
    return true;
  }

  template <int N, int N2, typename T>
  T TCPBoostSocketServer<N, N2, T>::accept(t_sockaddr *, int *)
  {
    std::shared_ptr<TCPBoostSocket<N, N2>>	socket;
    
    _acceptor.async_accept(socket->getSocket(),
			   boost::bind(&TCPBoostSocketServer::handleAccept<HandlerAsyncAccept<N, N2, T>>,
				       this,
				       socket,
				       boost::asio::placeholders::error));
    return socket;
  }

  template <int N, int N2, typename T>
  template <typename HandlerPolicy>
  void TCPBoostSocketServer<N, N2, T>::handleAccept(std::shared_ptr<TCPBoostSocket<N, N2>>& socket,
					     const boost::system::error_code& error)
  {
    HandlerPolicy::handleAccept(*this, socket, error);
  }

  template <int N, int N2, typename T>
  template <int M, int M2, typename U>
  void TCPBoostSocketServer<N, N2, T>::HandlerAsyncAccept<M, M2, U>::handleAccept(TCPBoostSocketServer<M, M2, U>&,
										  std::shared_ptr<TCPBoostSocket<M, M2>>&,
										  const boost::system::error_code&)
  {
  }
}

#endif /* !TCP_BOOST_SOCKET_SERVER_HPP_ */
