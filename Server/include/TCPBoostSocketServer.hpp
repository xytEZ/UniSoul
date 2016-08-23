#ifndef TCP_BOOST_SOCKET_SERVER_HPP_
# define TCP_BOOST_SOCKET_SERVER_HPP_

# include <cstddef>
# include <memory>
# include <map>
# include <boost/asio.hpp>
# include <boost/array.hpp>
# include <boost/noncopyable.hpp>
# include <boost/enable_shared_from_this.hpp>
# include <boost/date_time/posix_time/posix_time.hpp>
# include <boost/date_time/posix_time/posix_time_io.hpp>
# include <boost/any.hpp>
# include <boost/bind.hpp>

# include "AsyncAcceptHandler.hpp"
# include "AsyncReadHandler.hpp"
# include "AsyncWriteHandler.hpp"
# include "ITCPSocketServer.hpp"

namespace Network
{ 
  template <std::size_t N = 256, std::size_t N2 = 300>
  class TCPBoostSocketServer :
    public ITCPSocketServer<boost::asio::ip::tcp::socket,
			    std::shared_ptr
			    <ITCPSocket<boost::asio::ip::tcp::socket>>>,
    private boost::noncopyable,
    public std::enable_shared_from_this<TCPBoostSocketServer<N, N2>>
  {
  private :
    using SystemWrapperPtrRef = std::unique_ptr
      <Wrapper::IWrapper<std::map<std::string, boost::any>>>&;
    
  private :
    boost::asio::ip::tcp::socket	_socket;
    boost::asio::ip::tcp::acceptor	_acceptor;
    boost::asio::ip::tcp::endpoint	_endpoint;
    boost::asio::deadline_timer		_timer;
    boost::array<char, N>		_buffer;
    std::size_t				_bytesTransferred;
    RemoteConnectionInfo		_remoteConnectionInfo;
    SystemWrapperPtrRef			_systemWrapperPtrRef;
      
  public :
    template <std::size_t M = 256, std::size_t M2 = 300>
      static std::shared_ptr<TCPBoostSocketServer<M, M2>>
      create(boost::asio::io_service&,
	     SystemWrapperPtrRef,
	     const std::string&,
	     unsigned short,
	     const RemoteConnectionInfo& = RemoteConnectionInfo());
    
  private :
    template <std::size_t M = 256, std::size_t M2 = 300>
      static std::shared_ptr<TCPBoostSocketServer<M, M2>>
      create(boost::asio::io_service&,
	     SystemWrapperPtrRef,
	     const RemoteConnectionInfo& = RemoteConnectionInfo());
    
  private :
    TCPBoostSocketServer(boost::asio::io_service&,
			 SystemWrapperPtrRef,
			 const std::string&,
			 unsigned short,
			 const RemoteConnectionInfo&);
    
    TCPBoostSocketServer(boost::asio::io_service&,
			 SystemWrapperPtrRef,
			 const RemoteConnectionInfo&);
    
  public :
    virtual ~TCPBoostSocketServer() = default;
    virtual bool open();
    virtual bool close();
    virtual const boost::asio::ip::tcp::socket& getDescriptor() const;
    virtual bool send(const std::string&);
    virtual std::string recv();
    virtual std::string getAddress() const;
    virtual unsigned short getPort() const;
    virtual const RemoteConnectionInfo& getRemoteConnectionInfo() const;
    virtual void setRemoteConnectionInfo(const RemoteConnectionInfo&);
    virtual bool bind();
    virtual bool listen();
    virtual std::shared_ptr<ITCPSocket<boost::asio::ip::tcp::socket>> accept();
    std::string getBuffer() const;
    SystemWrapperPtrRef getSystemWrapperPtrRef();

  private :
    template <typename HandlerPolicy>
      void acceptHandle(const std::shared_ptr<TCPBoostSocketServer<N, N2>>&,
			const std::shared_ptr
			<ITCPSocket<boost::asio::ip::tcp::socket>>&,
			const boost::system::error_code&);

    template <typename HandlerPolicy>
      void sendHandle(const std::shared_ptr
		      <ITCPSocket<boost::asio::ip::tcp::socket>>&,
		      const boost::system::error_code&,
		      std::size_t);
    
    template <typename HandlerPolicy>
      void recvHandle(const std::shared_ptr
		      <ITCPSocket<boost::asio::ip::tcp::socket>>&,
		      const boost::system::error_code&,
		      std::size_t);
  };

  template <std::size_t N, std::size_t N2>
  template <std::size_t M, std::size_t M2>
  std::shared_ptr<TCPBoostSocketServer<M, M2>>
    TCPBoostSocketServer<N, N2>
    ::create(boost::asio::io_service& ios,
	     SystemWrapperPtrRef systemWrapperPtrRef,
	     const std::string& hostname,
	     unsigned short port,
	     const RemoteConnectionInfo& remoteConnectionInfo)
  {
    return std::shared_ptr
      <TCPBoostSocketServer<M, M2>>
      (new TCPBoostSocketServer<M, M2>(ios,
				       systemWrapperPtrRef,
				       hostname,
				       port,
				       remoteConnectionInfo));
  }

  template <std::size_t N, std::size_t N2>
  template <std::size_t M, std::size_t M2>
  std::shared_ptr<TCPBoostSocketServer<M, M2>>
    TCPBoostSocketServer<N, N2>
    ::create(boost::asio::io_service& ios,
	     SystemWrapperPtrRef systemWrapperPtrRef,
	     const RemoteConnectionInfo& remoteConnectionInfo)
  {
    return std::shared_ptr
      <TCPBoostSocketServer<M, M2>>
      (new TCPBoostSocketServer<M, M2>(ios,
				       systemWrapperPtrRef,
				       remoteConnectionInfo));
  }
  
  template <std::size_t N, std::size_t N2>
  TCPBoostSocketServer<N, N2>
  ::TCPBoostSocketServer(boost::asio::io_service& ios,
			 SystemWrapperPtrRef systemWrapperPtrRef,
			 const std::string& hostname,
			 unsigned short port,
			 const RemoteConnectionInfo& remoteConnectionInfo) :
    _socket(ios),
    _acceptor(ios),
    _endpoint(boost::asio::ip::address::from_string(hostname), port),
    _timer(ios, boost::posix_time::seconds(N2)),
    _remoteConnectionInfo(remoteConnectionInfo),
    _systemWrapperPtrRef(systemWrapperPtrRef)
  {
  }
  
  template <std::size_t N, std::size_t N2>
  TCPBoostSocketServer<N, N2>
  ::TCPBoostSocketServer(boost::asio::io_service& ios,
			 SystemWrapperPtrRef systemWrapperPtrRef,
			 const RemoteConnectionInfo& remoteConnectionInfo) :
    _socket(ios),
    _acceptor(ios),
    _endpoint(),
    _timer(ios, boost::posix_time::seconds(N2)),
    _remoteConnectionInfo(remoteConnectionInfo),
    _systemWrapperPtrRef(systemWrapperPtrRef)
  {
  }

  template <std::size_t N, std::size_t N2>
  bool TCPBoostSocketServer<N, N2>::open()
  {
    _acceptor.open(_endpoint.protocol());
    _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    return true;
  }

  template <std::size_t N, std::size_t N2>
  bool TCPBoostSocketServer<N, N2>::close()
  {
    _socket.close();
    return true;
  }

  template <std::size_t N, std::size_t N2>
  const boost::asio::ip::tcp::socket&
  TCPBoostSocketServer<N, N2>::getDescriptor() const
  {
    return _socket;
  }
  
  template <std::size_t N, std::size_t N2>
  bool TCPBoostSocketServer<N, N2>::send(const std::string& msg)
  {
    boost::asio::async_write
      (_socket,
       boost::asio::buffer(msg, msg.size()),
       boost::bind(&TCPBoostSocketServer::sendHandle
		   <Network::AsyncWriteHandler<N, N2>>,
		   this->shared_from_this(),
		   this->shared_from_this(),
		   boost::asio::placeholders::error,
		   boost::asio::placeholders::bytes_transferred));
    return true;
  }

  template <std::size_t N, std::size_t N2>
  std::string TCPBoostSocketServer<N, N2>::recv()
  {
    _buffer.assign(0);
    boost::asio::async_read
      (_socket,
       boost::asio::buffer(_buffer),
       boost::bind(&TCPBoostSocketServer::recvHandle
		   <Network::AsyncReadHandler<N, N2>>,
		   this->shared_from_this(),
		   this->shared_from_this(),
		   boost::asio::placeholders::error,
		   boost::asio::placeholders::bytes_transferred));
    /*_timer.async_wait(boost::bind(&TCPBoostSocketServer::close,
      this->shared_from_this()));*/
    return "";
  }

  template <std::size_t N, std::size_t N2>
  std::string TCPBoostSocketServer<N, N2>::getAddress() const
  {
    return _socket.remote_endpoint().address().to_string();
  }

  template <std::size_t N, std::size_t N2>
  unsigned short TCPBoostSocketServer<N, N2>::getPort() const
  {
    return _socket.remote_endpoint().port();
  }

  template <std::size_t N, std::size_t N2>
  const RemoteConnectionInfo&
  TCPBoostSocketServer<N, N2>::getRemoteConnectionInfo() const
  {
    return _remoteConnectionInfo;
  }

  template <std::size_t N, std::size_t N2>
  void TCPBoostSocketServer<N, N2>
  ::setRemoteConnectionInfo(const RemoteConnectionInfo& remoteConnectionInfo)
  {
    _remoteConnectionInfo = remoteConnectionInfo;
  }
  
  template <std::size_t N, std::size_t N2>
  bool TCPBoostSocketServer<N, N2>::bind()
  {
    _acceptor.bind(_endpoint);
    return true;
  }
  
  template <std::size_t N, std::size_t N2>
  bool TCPBoostSocketServer<N, N2>::listen()
  {
    _acceptor.listen();
    return true;
  }
  
  template <std::size_t N, std::size_t N2>
  std::shared_ptr<ITCPSocket<boost::asio::ip::tcp::socket>>
  TCPBoostSocketServer<N, N2>::accept()
  {
    std::shared_ptr<ITCPSocket<boost::asio::ip::tcp::socket>>	socketPtr =
      TCPBoostSocketServer<N, N2>::template create<N, N2>
      (_socket.get_io_service(), _systemWrapperPtrRef);
    
    _acceptor.async_accept(const_cast<boost::asio::ip::tcp::socket&>
			   (socketPtr->getDescriptor()),
			   boost::bind(&TCPBoostSocketServer::acceptHandle
				       <Network::AsyncAcceptHandler<N, N2>>,
				       this,
				       this->shared_from_this(),
				       socketPtr,
				       boost::asio::placeholders::error));
    return socketPtr;
  }

  template <std::size_t N, std::size_t N2>
  std::string TCPBoostSocketServer<N, N2>::getBuffer() const
  {
    return std::string(_buffer.data(), _bytesTransferred);
  }
  
  template <std::size_t N, std::size_t N2>
  typename TCPBoostSocketServer<N, N2>::SystemWrapperPtrRef
  TCPBoostSocketServer<N, N2>::getSystemWrapperPtrRef()
  {
    return _systemWrapperPtrRef;
  }
  
  template <std::size_t N, std::size_t N2>
  template <typename HandlerPolicy>
  void TCPBoostSocketServer<N, N2>
  ::acceptHandle(const std::shared_ptr
		 <TCPBoostSocketServer<N, N2>>& serverSocketPtr,
		 const std::shared_ptr<ITCPSocket
		 <boost::asio::ip::tcp::socket>>&
		 socketPtr,
		 const boost::system::error_code& error)
  {
    HandlerPolicy(serverSocketPtr, socketPtr, error).acceptHandle();
  }

  template <std::size_t N, std::size_t N2>
  template <typename HandlerPolicy>
  void TCPBoostSocketServer<N, N2>
  ::sendHandle(const std::shared_ptr
	       <ITCPSocket<boost::asio::ip::tcp::socket>>& socketPtr,
	       const boost::system::error_code& error,
	       std::size_t)
  {
    HandlerPolicy(socketPtr, error).writeHandle();
  }

  template <std::size_t N, std::size_t N2>
  template <typename HandlerPolicy>
  void TCPBoostSocketServer<N, N2>
  ::recvHandle(const std::shared_ptr
	       <ITCPSocket<boost::asio::ip::tcp::socket>>& socketPtr,
	       const boost::system::error_code& error,
	       std::size_t bytes_transferred)
  {
    _bytesTransferred = bytes_transferred;
    HandlerPolicy(socketPtr, error).readHandle();
  }
}

#endif /* !TCP_BOOST_SOCKET_SERVER_HPP_ */
