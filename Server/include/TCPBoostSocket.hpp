#ifndef TCP_BOOST_SOCKET_HPP_
# define TCP_BOOST_SOCKET_HPP_

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

# include "ITCPSocket.hpp"
# include "AsyncReadHandler.hpp"
# include "AsyncWriteHandler.hpp"
# include "Descriptor.hh"

namespace Network
{
  template <std::size_t N, std::size_t N2>
  class TCPBoostSocket : virtual public ITCPSocket<Descriptor>,
			 private boost::noncopyable,
			 public std::enable_shared_from_this
    <TCPBoostSocket<N, N2>>
  { 
  protected :
    using SystemWrapperPtrRef = std::unique_ptr
      <Wrapper::IWrapper<std::map<std::string, boost::any>>>&;
    
  protected :
    boost::asio::ip::tcp::socket	_socket;
    Descriptor				_descriptor;
    boost::asio::deadline_timer		_timer;
    boost::array<char, N>		_buffer;
    SystemWrapperPtrRef			_systemWrapperPtrRef;

  public :
    template <std::size_t M, std::size_t M2>
      static std::shared_ptr<TCPBoostSocket<M, M2>>
      create(boost::asio::io_service&,
	     SystemWrapperPtrRef);
    
  protected :
    TCPBoostSocket(boost::asio::io_service&, SystemWrapperPtrRef);
    
  public :
    virtual ~TCPBoostSocket() = default;
    virtual bool open();
    virtual bool close();
    virtual const ::Descriptor& getDescriptor() const;
    virtual bool send(const std::string&);
    virtual std::string recv();
    virtual std::string getAddress() const;
    virtual unsigned short getPort() const;
    boost::asio::ip::tcp::socket& getSocket();
    std::string getBuffer() const;
    SystemWrapperPtrRef getSystemWrapperPtrRef();

  private :
    template <typename HandlerPolicy>
      void sendHandle(const std::shared_ptr<TCPBoostSocket<N, N2>>&,
		      const boost::system::error_code&);
    
    template <typename HandlerPolicy>
      void recvHandle(const std::shared_ptr<TCPBoostSocket<N, N2>>&,
		      const boost::system::error_code&);
  };

  template <std::size_t N, std::size_t N2>
  template <std::size_t M, std::size_t M2>
  std::shared_ptr<TCPBoostSocket<M, M2>> TCPBoostSocket<N, N2>
    ::create(boost::asio::io_service& ios,
	     SystemWrapperPtrRef systemWrapperPtrRef)
  {
    return std::shared_ptr
      <TCPBoostSocket<M, M2>>(new TCPBoostSocket
			      <M, M2>(ios, systemWrapperPtrRef));
  }
  
  template <std::size_t N, std::size_t N2>
  TCPBoostSocket<N, N2>
  ::TCPBoostSocket(boost::asio::io_service& ios,
		   SystemWrapperPtrRef systemWrapperPtrRef) :
    _socket(ios),
    _descriptor(ios),
    _timer(ios, boost::posix_time::seconds(N2)),
    _systemWrapperPtrRef(systemWrapperPtrRef)
  {
  }

  template <std::size_t N, std::size_t N2>
  bool TCPBoostSocket<N, N2>::open() { return true; }

  template <std::size_t N, std::size_t N2>
  bool TCPBoostSocket<N, N2>::close()
  {
    _socket.close();
    return true;
  }
  
  template <std::size_t N, std::size_t N2>
  const ::Descriptor& TCPBoostSocket<N, N2>::getDescriptor() const
  {
    return _descriptor;
  }
  
  template <std::size_t N, std::size_t N2>
  bool TCPBoostSocket<N, N2>::send(const std::string& msg)
  {
    boost::asio::async_write(_socket,
			     boost::asio::buffer(msg),
			     boost
			     ::bind(&TCPBoostSocket
				    ::sendHandle
				    <Handler::AsyncWriteHandler<N, N2>>,
				    this->shared_from_this(),
				    this->shared_from_this(),
				    boost::asio::placeholders::error));
    return true;
  }
  
  template <std::size_t N, std::size_t N2>
  std::string TCPBoostSocket<N, N2>::recv()
  {
    boost::asio::async_read(_socket,
			    boost::asio::buffer(_buffer),
			    boost
			    ::bind(&TCPBoostSocket
				   ::recvHandle
				   <Handler::AsyncReadHandler<N, N2>>,
				   this->shared_from_this(),
				   this->shared_from_this(),
				   boost::asio::placeholders::error));
    _timer.expires_from_now(boost::posix_time::seconds(N2));
    _timer.async_wait(boost::bind(&TCPBoostSocket::close,
				  this->shared_from_this()));
    return "";
  }
  
  template <std::size_t N, std::size_t N2>
  std::string TCPBoostSocket<N, N2>::getAddress() const
  {
    return _socket.remote_endpoint().address().to_string();
  }

  template <std::size_t N, std::size_t N2>
  unsigned short TCPBoostSocket<N, N2>::getPort() const
  {
    return _socket.remote_endpoint().port();
  }
  
  template <std::size_t N, std::size_t N2>
  boost::asio::ip::tcp::socket& TCPBoostSocket<N, N2>::getSocket()
  {
    return _socket;
  }

  template <std::size_t N, std::size_t N2>
  std::string TCPBoostSocket<N, N2>::getBuffer() const
  {
    return std::string(_buffer.cbegin(), _buffer.end());
  }

  template <std::size_t N, std::size_t N2>
  typename TCPBoostSocket<N, N2>::SystemWrapperPtrRef
  TCPBoostSocket<N, N2>::getSystemWrapperPtrRef()
  {
    return _systemWrapperPtrRef;
  }
  
  template <std::size_t N, std::size_t N2>
  template <typename HandlerPolicy>
  void TCPBoostSocket<N, N2>::sendHandle(const std::shared_ptr
					 <TCPBoostSocket<N, N2>>& socketPtr,
					 const boost::system::error_code&
					 error)
  {
    HandlerPolicy(socketPtr, error).writeHandle();
  }
  
  template <std::size_t N, std::size_t N2>
  template <typename HandlerPolicy>
  void TCPBoostSocket<N, N2>::recvHandle(const std::shared_ptr
					 <TCPBoostSocket<N, N2>>& socketPtr,
					 const boost::system::error_code&
					 error)
  {
    HandlerPolicy(socketPtr, error).readHandle();
  }
}

#endif /* !TCP_BOOST_SOCKET_HPP_ */
