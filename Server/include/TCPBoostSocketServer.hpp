#ifndef TCP_BOOST_SOCKET_SERVER_HPP_
# define TCP_BOOST_SOCKET_SERVER_HPP_

# include "TCPBoostSocket.hpp"
# include "ITCPSocketServer.hpp"
# include "AsyncAcceptHandler.hpp"
# include "Descriptor.hh"

namespace Network
{ 
  template <std::size_t N, std::size_t N2, typename T = TCPBoostSocket<N, N2>>
    class TCPBoostSocketServer : public TCPBoostSocket<N, N2>,
				 public ITCPSocketServer<Descriptor, T>
    { 
    private :
      boost::asio::ip::tcp::acceptor	_acceptor;
      boost::asio::ip::tcp::endpoint	_endpoint;
      
    public :
      template <std::size_t M, std::size_t M2, typename U>
      static std::shared_ptr<TCPBoostSocketServer<M, M2, U>>
	create(boost::asio::io_service&,
	       typename TCPBoostSocket<M, M2>
	       ::SystemWrapperPtrRef,
	       const std::string&,
	       unsigned short);
      
    protected :
      TCPBoostSocketServer(boost::asio::io_service&,
			   typename TCPBoostSocket<N, N2>
			   ::SystemWrapperPtrRef,
			   const std::string&,
			   unsigned short);
      
    public :
      virtual ~TCPBoostSocketServer() = default;
      virtual bool open(int, int, int);
      virtual bool close();
      virtual const ::Descriptor& getDescriptor() const;
      virtual bool send(const std::string&);
      virtual std::string recv();
      virtual std::string getAddress() const;
      virtual unsigned short getPort() const;
      virtual bool bind(const t_sockaddr *, int);
      virtual bool listen(int);
      virtual T accept(t_sockaddr *, int *);

    private :
      template <typename HandlerPolicy>
      void acceptHandle(const std::shared_ptr<TCPBoostSocketServer<N, N2, T>>&,
			const std::shared_ptr<TCPBoostSocket<N, N2>>&,
			const boost::system::error_code&);
    };

  template <std::size_t N, std::size_t N2, typename T>
  template <std::size_t M, std::size_t M2, typename U>
  std::shared_ptr<TCPBoostSocketServer<M, M2, U>>
    TCPBoostSocketServer<N, N2, T>
    ::create(boost::asio::io_service& ios,
	     typename TCPBoostSocket<M, M2>
	     ::SystemWrapperPtrRef systemWrapperPtrRef,
	     const std::string& hostname,
	     unsigned short port)
  {
    return std::shared_ptr
      <TCPBoostSocketServer<M, M2, U>>
      (new TCPBoostSocketServer<M, M2, U>(ios,
					  systemWrapperPtrRef,
					  hostname,
					  port));
  }
  
  template <std::size_t N, std::size_t N2, typename T>
  TCPBoostSocketServer<N, N2, T>
  ::TCPBoostSocketServer(boost::asio::io_service& ios,
			 typename TCPBoostSocket<N, N2>
			 ::SystemWrapperPtrRef systemWrapperPtrRef,
			 const std::string& hostname,
			 unsigned short port) :
    TCPBoostSocket<N, N2>(ios, systemWrapperPtrRef),
    _acceptor(ios),
    _endpoint(boost::asio::ip::address::from_string(hostname), port)
  {
  }

  template <std::size_t N, std::size_t N2, typename T>
  bool TCPBoostSocketServer<N, N2, T>::open(int, int, int)
  {
    _acceptor.open(_endpoint.protocol());
    _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    return true;
  }

  template <std::size_t N, std::size_t N2, typename T>
  bool TCPBoostSocketServer<N, N2, T>::close()
  {
    return TCPBoostSocket<N, N2>::close();
  }

  template <std::size_t N, std::size_t N2, typename T>
  const ::Descriptor& TCPBoostSocketServer<N, N2, T>::getDescriptor() const
  {
    return TCPBoostSocket<N, N2>::getDescriptor();
  }
  
  template <std::size_t N, std::size_t N2, typename T>
  bool TCPBoostSocketServer<N, N2, T>::send(const std::string& msg)
  {
    return TCPBoostSocket<N, N2>::send(msg);
  }

  template <std::size_t N, std::size_t N2, typename T>
  std::string TCPBoostSocketServer<N, N2, T>::recv()
  {
    return TCPBoostSocket<N, N2>::recv();
  }

  template <std::size_t N, std::size_t N2, typename T>
  std::string TCPBoostSocketServer<N, N2, T>::getAddress() const
  {
    return TCPBoostSocket<N, N2>::getAddress();
  }

  template <std::size_t N, std::size_t N2, typename T>
  unsigned short TCPBoostSocketServer<N, N2, T>::getPort() const
  {
    return TCPBoostSocket<N, N2>::getPort();
  }
  
  template <std::size_t N, std::size_t N2, typename T>
  bool TCPBoostSocketServer<N, N2, T>::bind(const t_sockaddr *, int)
  {
    _acceptor.bind(_endpoint);
    return true;
  }
  
  template <std::size_t N, std::size_t N2, typename T>
  bool TCPBoostSocketServer<N, N2, T>::listen(int)
  {
    _acceptor.listen();
    return true;
  }
  
  template <std::size_t N, std::size_t N2, typename T>
  T TCPBoostSocketServer<N, N2, T>::accept(t_sockaddr *, int *)
  {
    std::shared_ptr<TCPBoostSocket<N, N2>>	socketPtr =
      TCPBoostSocket<N, N2>::template create<N, N2>
      (this->_socket.get_io_service(), this->_systemWrapperPtrRef);

    _acceptor.async_accept(socketPtr->getSocket(),
			   boost
			   ::bind(&TCPBoostSocketServer
				  ::acceptHandle
				  <Handler::AsyncAcceptHandler<N, N2, T>>,
				  this,
				  std::static_pointer_cast
				  <TCPBoostSocketServer<N, N2, T>>
				  (this->shared_from_this()),
				  socketPtr,
				  boost::asio::placeholders::error));
    return socketPtr;
  }

  template <std::size_t N, std::size_t N2, typename T>
  template <typename HandlerPolicy>
  void TCPBoostSocketServer<N, N2, T>
  ::acceptHandle(const std::shared_ptr
		 <TCPBoostSocketServer<N, N2, T>>& serverSocketPtr,
		 const std::shared_ptr<TCPBoostSocket<N, N2>>& socketPtr,
		 const boost::system::error_code& error)
  {
    HandlerPolicy(serverSocketPtr, socketPtr, error).acceptHandle();
  }
}

#endif /* !TCP_BOOST_SOCKET_SERVER_HPP_ */
