#ifndef TCP_BOOST_SOCKET_SERVER_HPP_
# define TCP_BOOST_SOCKET_SERVER_HPP_

# include <utility>
# include <boost/bind.hpp>
# include "TCPBoostSocket.hpp"
# include "ITCPSocketServer.hpp"

namespace Network
{
  template <int N, typename T = TCPBoostSocket<N>>
    class TCPBoostSocketServer : public TCPBoostSocket<N>,
				 public ITCPSocketServer<T>
  {
  private :
    boost::asio::ip::tcp::acceptor	_acceptor;
    boost::asio::ip::tcp::endpoint	_endpoint;
    
  public :
    TCPBoostSocketServer(boost::asio::io_service&,
			 typename TCPBoostSocket<N>::ComplexSystem&,
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
    void handleAccept(std::shared_ptr<TCPBoostSocket<N>>&,
		      const boost::system::error_code&);
  };

  template <int N, typename T>
  TCPBoostSocketServer<N, T>::TCPBoostSocketServer(boost::asio::io_service& ios,
						   typename TCPBoostSocket<N>::ComplexSystem& complexSystem,
						   const std::string& hostname,
						   int port) :
    TCPBoostSocket<N>(ios, complexSystem),
    _acceptor(ios),
    _endpoint(boost::asio::ip::address::from_string(std::move(hostname)), port)
  {
  }

  template <int N, typename T>
  TCPBoostSocketServer<N, T>::~TCPBoostSocketServer() { }

  template <int N, typename T>
  bool TCPBoostSocketServer<N, T>::open(int, int, int)
  {
    _acceptor.open(_endpoint.protocol());
    _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    return true;
  }

  template <int N, typename T>
  bool TCPBoostSocketServer<N, T>::close()
  {
    return TCPBoostSocket<N>::close();
  }

  template <int N, typename T>
  bool TCPBoostSocketServer<N, T>::send(const std::string& msg)
  {
    return TCPBoostSocket<N>::send(msg);
  }

  template <int N, typename T>
  std::string TCPBoostSocketServer<N, T>::recv()
  {
    return TCPBoostSocket<N>::recv();
  }
  
  template <int N, typename T>
  bool TCPBoostSocketServer<N, T>::bind(const t_sockaddr *, int)
  {
    _acceptor.bind(_endpoint);
    return true;
  }

  template <int N, typename T>
  bool TCPBoostSocketServer<N, T>::listen(int)
  {
    _acceptor.listen();
    return true;
  }

  template <int N, typename T>
  T TCPBoostSocketServer<N, T>::accept(t_sockaddr *, int *)
  {
    std::shared_ptr<TCPBoostSocket<N>>	socket;
    
    _acceptor.async_accept(socket->getSocket(),
			   boost::bind(&TCPBoostSocketServer::handleAccept,
				       this,
				       socket,
				       boost::asio::placeholders::error));
    return socket;
  }

  template <int N, typename T>
  void TCPBoostSocketServer<N, T>::handleAccept(std::shared_ptr<TCPBoostSocket<N>>& socket,
					     const boost::system::error_code& error)
  {
    if (!error)
      {
	socket->send("Welcome to the server");
	accept(nullptr, nullptr);
      }
  }
}

#endif /* !TCP_BOOST_SOCKET_SERVER_HPP_ */
