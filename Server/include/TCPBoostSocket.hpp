#ifndef TCP_BOOST_SOCKET_HPP_
# define TCP_BOOST_SOCKET_HPP_

# include <memory>
# include <boost/asio.hpp>
# include <boost/array.hpp>
# include <boost/noncopyable.hpp>
# include <boost/enable_shared_from_this.hpp>
# include <boost/date_time/posix_time/posix_time.hpp>
# include <boost/date_time/posix_time/posix_time_io.hpp>
# include "ITCPSocket.hh"

namespace Wrapper
{
  namespace System
  {
    class ISystemWrapper;
  }
}

namespace Network
{
  template <int N, int N2>
  class TCPBoostSocket : virtual public ITCPSocket,
			 private boost::noncopyable,
			 public boost
    ::enable_shared_from_this<TCPBoostSocket<N, N2>>
  {
  private :
    template <int M, int M2>
      struct HandlerAsyncRead
    {
      static void handleRead(TCPBoostSocket<M, M2>&,
			     const boost::system::error_code&);
    };
    
    template <int M, int M2>
      struct HandlerAsyncWrite
    {
      static void handleWrite(TCPBoostSocket<M, M2>&,
			      const boost::system::error_code&);
    };
    
  protected :
    using SystemWrapperRef = std::unique_ptr
      <Wrapper::System::ISystemWrapper>&;
    
  private :
    boost::asio::ip::tcp::socket	_socket;
    boost::asio::deadline_timer		_timer;
    std::string				_msg;
    boost::array<char, N>		_buffer;

  protected :
    boost::asio::io_service&		_ios;
    SystemWrapperRef			_systemWrapperRef;
    
  public :
    TCPBoostSocket(boost::asio::io_service&, SystemWrapperRef&);
    virtual ~TCPBoostSocket();
    virtual bool open(int, int, int);
    virtual bool close();
    virtual bool send(const std::string&);
    virtual std::string recv();
    boost::asio::ip::tcp::socket& getSocket();

  private :
    template <typename HandlerPolicy>
    void handleSend(const boost::system::error_code&);

    template <typename HandlerPolicy>
    void handleRecv(const boost::system::error_code&);
  };

  template <int N, int N2>
  TCPBoostSocket<N, N2>::TCPBoostSocket(boost::asio::io_service& ios,
					SystemWrapperRef systemWrapperRef) :
    _socket(ios),
    _timer(ios, boost::posix_time::seconds(N2)),
    _ios(ios),
    _systemWrapperRef(systemWrapperRef)
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
				    ::handleSend<HandlerAsyncWrite<N, N2>>,
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
				   ::handleRecv<HandlerAsyncRead<N, N2>>,
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
  template <typename HandlerPolicy>
  void TCPBoostSocket<N, N2>::handleSend(const boost::system::error_code& error)
  {
    HandlerPolicy::handleWrite(*this, error);
  }
  
  template <int N, int N2>
  template <typename HandlerPolicy>
  void TCPBoostSocket<N, N2>::handleRecv(const boost::system::error_code& error)
  {
    HandlerPolicy::handleRead(*this, error);
  }

  template <int N, int N2>
  template <int M, int M2>
  void TCPBoostSocket<N, N2>::HandlerAsyncRead<M, M2>
  ::handleRead(TCPBoostSocket<M, M2>&,
	       const boost::system::error_code&)
  {
  }

  template <int N, int N2>
  template <int M, int M2>
  void TCPBoostSocket<N, N2>::HandlerAsyncWrite<M, M2>
  ::handleWrite(TCPBoostSocket<M, M2>&,
		const boost::system::error_code&)
  {
  }
}

#endif /* !TCP_BOOST_SOCKET_HPP_ */
