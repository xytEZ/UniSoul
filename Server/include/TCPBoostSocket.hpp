#ifndef TCP_BOOST_SOCKET_HPP_
# define TCP_BOOST_SOCKET_HPP_

# include <memory>
# include <boost/asio.hpp>
# include <boost/array.hpp>
# include <boost/noncopyable.hpp>
# include <boost/enable_shared_from_this.hpp>
# include <boost/date_time/posix_time/posix_time.hpp>
# include <boost/date_time/posix_time/posix_time_io.hpp>
# include <boost/any.hpp>
# include <boost/bind.hpp>
# include "ITCPSocket.hh"
# include "HandlerAsyncRead.hpp"
# include "HandlerAsyncWrite.hpp"

namespace Network
{
  template <int N, int N2>
  class TCPBoostSocket : virtual public ITCPSocket,
			 private boost::noncopyable,
			 public std::enable_shared_from_this
    <TCPBoostSocket<N, N2>>
  { 
  protected :
    using SystemWrapperPtrRef = std::unique_ptr
      <Wrapper::System::ISystemWrapper>&;
    
  private :
    boost::asio::ip::tcp::socket	_socket;
    boost::asio::deadline_timer		_timer;
    std::string				_msg;
    boost::array<char, N>		_buffer;

  protected :
    boost::asio::io_service&		_ios;
    SystemWrapperPtrRef			_systemWrapperPtrRef;

  public :
    template <int M, int M2>
      static std::shared_ptr<TCPBoostSocket<M, M2>>
      create(boost::asio::io_service&,
	     SystemWrapperPtrRef);
    
  protected :
    TCPBoostSocket(boost::asio::io_service&, SystemWrapperPtrRef);
    
  public :
    virtual ~TCPBoostSocket();
    virtual bool open(int, int, int);
    virtual bool close();
    virtual bool send(const std::string&);
    virtual std::string recv();
    boost::asio::ip::tcp::socket& getSocket();
    SystemWrapperPtrRef getSystemWrapperPtrRef();

  private :
    template <typename HandlerPolicy>
      void handleSend(std::shared_ptr<TCPBoostSocket<N, N2>>,
		      const boost::system::error_code&);
    
    template <typename HandlerPolicy>
      void handleRecv(std::shared_ptr<TCPBoostSocket<N, N2>>,
		      const boost::system::error_code&);
  };

  template <int N, int N2>
  template <int M, int M2>
  std::shared_ptr<TCPBoostSocket<M, M2>> TCPBoostSocket<N, N2>
    ::create(boost::asio::io_service& ios,
	     SystemWrapperPtrRef systemWrapperPtrRef)
  {
    return std::shared_ptr
      <TCPBoostSocket<M, M2>>(new TCPBoostSocket<M, M2>(ios, systemWrapperPtrRef));
  }
  
  template <int N, int N2>
  TCPBoostSocket<N, N2>::TCPBoostSocket(boost::asio::io_service& ios,
					SystemWrapperPtrRef systemWrapperPtrRef) :
    _socket(ios),
    _timer(ios, boost::posix_time::seconds(N2)),
    _ios(ios),
    _systemWrapperPtrRef(systemWrapperPtrRef)
  {
  }

  template <int N, int N2>
  TCPBoostSocket<N, N2>::~TCPBoostSocket() { }

  template <int N, int N2>
  bool TCPBoostSocket<N, N2>::open(int, int, int) { return true; }

  template <int N, int N2>
  bool TCPBoostSocket<N, N2>::close()
  {
    _socket.close();
    return true;
  }

  template <int N, int N2>
  bool TCPBoostSocket<N, N2>::send(const std::string& msg)
  {
    _msg = std::move(msg);
    boost::asio::async_write(_socket,
			     boost::asio::buffer(_msg),
			     boost
			     ::bind(&TCPBoostSocket
				    ::handleSend
				    <Handler::HandlerAsyncWrite<N, N2>>,
				    this->shared_from_this(),
				    this->shared_from_this(),
				    boost::asio::placeholders::error));
    return true;
  }

  template <int N, int N2>
  std::string TCPBoostSocket<N, N2>::recv()
  {
    boost::asio::async_read(_socket,
			    boost::asio::buffer(_buffer),
			    boost
			    ::bind(&TCPBoostSocket
				   ::handleRecv
				   <Handler::HandlerAsyncRead<N, N2>>,
				   this->shared_from_this(),
				   this->shared_from_this(),
				   boost::asio::placeholders::error));
    _timer.expires_from_now(boost::posix_time::seconds(N2));
    _timer.async_wait(boost::bind(&TCPBoostSocket::close,
				  this->shared_from_this()));
    return "";
  }
  
  template <int N, int N2>
  boost::asio::ip::tcp::socket& TCPBoostSocket<N, N2>::getSocket()
  {
    return _socket;
  }

  template <int N, int N2>
  typename TCPBoostSocket<N, N2>::SystemWrapperPtrRef
  TCPBoostSocket<N, N2>::getSystemWrapperPtrRef()
  {
    return _systemWrapperPtrRef;
  }
  
  template <int N, int N2>
  template <typename HandlerPolicy>
  void TCPBoostSocket<N, N2>::handleSend(std::shared_ptr<TCPBoostSocket<N, N2>> socket,
					 const boost::system::error_code& error)
  {
    HandlerPolicy::handleWrite(socket, error);
  }
  
  template <int N, int N2>
  template <typename HandlerPolicy>
  void TCPBoostSocket<N, N2>::handleRecv(std::shared_ptr<TCPBoostSocket<N, N2>> socket,
					 const boost::system::error_code& error)
  {
    HandlerPolicy::handleRead(socket, error);
  }
}

#endif /* !TCP_BOOST_SOCKET_HPP_ */
