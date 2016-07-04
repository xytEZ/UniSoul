#ifndef TCP_BOOST_SOCKET_HPP_
# define TCP_BOOST_SOCKET_HPP_

# include <boost/asio.hpp>
# include <boost/array.hpp>
# include <boost/noncopyable.hpp>
# include <boost/enable_shared_from_this.hpp>
# include <boost/date_time/posix_time/posix_time.hpp>
# include <boost/date_time/posix_time/posix_time_io.hpp>
# include "ITCPSocket.hh"

namespace Network
{
  template <int N>
  class TCPBoostSocket : virtual public ITCPSocket,
			 private boost::noncopyable,
			 public boost::enable_shared_from_this<TCPBoostSocket<N>>
  {
  private :
    boost::asio::ip::tcp::socket	_socket;
    boost::asio::deadline_timer		_timer;
    std::string				_msg;
    boost::array<char, N>		_buffer;
    
  public :
    TCPBoostSocket(boost::asio::io_service&);
    virtual ~TCPBoostSocket();
    virtual bool open(int, int, int);
    virtual bool close();
    virtual bool send(const std::string&);
    virtual std::string recv();
    boost::asio::ip::tcp::socket& getSocket();

  private :
    void handleSend(const boost::system::error_code&);
    void handleRecv(const boost::system::error_code&);
  };

  template <int N>
  TCPBoostSocket<N>::TCPBoostSocket(boost::asio::io_service& ios) :
    _socket(ios),
    _timer(ios, boost::posix_time::seconds(5))
  {
  }

  template <int N>
  TCPBoostSocket<N>::~TCPBoostSocket() { }

  template <int N>
  bool TCPBoostSocket<N>::open(int, int, int) { return true; }

  template <int N>
  bool TCPBoostSocket<N>::close()
  {
    _socket.close();
    return true;
  }

  template <int N>
  bool TCPBoostSocket<N>::send(const std::string& msg)
  {
    _msg = std::move(msg);
    boost::asio::async_write(_socket,
			     boost::asio::buffer(_msg),
			     boost::bind(&TCPBoostSocket::handleSend,
					 this->shared_from_this(),
					 boost::asio::placeholders::error));
    return true;
  }

  template <int N>
  std::string TCPBoostSocket<N>::recv()
  {
    boost::asio::async_read(_socket,
			    boost::asio::buffer(_buffer),
			    boost::bind(&TCPBoostSocket::handleRecv,
					this->shared_from_this(),
					boost::asio::placeholders::error));
    _timer.expires_from_now(boost::posix_time::seconds(5));
    _timer.async_wait(boost::bind(&TCPBoostSocket::close, this->shared_from_this()));
    return "";
  }

  template <int N>
  boost::asio::ip::tcp::socket& TCPBoostSocket<N>::getSocket()
  {
    return _socket;
  }

  template <int N>
  void TCPBoostSocket<N>::handleSend(const boost::system::error_code&)
  {
  }

  template <int N>
  void TCPBoostSocket<N>::handleRecv(const boost::system::error_code& error)
  {
    if (!error)
      {
	recv();
      }
  }
}

#endif /* !TCP_BOOST_SOCKET_HPP_ */
