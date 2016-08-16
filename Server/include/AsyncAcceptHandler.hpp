#ifndef ASYNC_ACCEPT_HANDLER_HPP_
# define ASYNC_ACCEPT_HANDLER_HPP_

# include <cstddef>
# include <memory>
# include <boost/system/error_code.hpp>

# include "ITCPSocket.hpp"
# include "UniSoulSystemWrapper.hh"

namespace Network
{
  template <std::size_t N, std::size_t N2>
  class TCPBoostSocketServer;
  
  template <std::size_t N, std::size_t N2>
  class AsyncAcceptHandler
  {
  private :
    std::shared_ptr
    <Network::TCPBoostSocketServer<N, N2>>	_serverSocketPtr;
    
    std::shared_ptr
    <Network::ITCPSocket
    <boost::asio::ip::tcp::socket>>		_socketPtr;
    
    boost::system::error_code			_error;
    
  public :
    AsyncAcceptHandler(const std::shared_ptr
		       <Network::TCPBoostSocketServer<N, N2>>&,
		       const std::shared_ptr
		       <Network::ITCPSocket
		       <boost::asio::ip::tcp::socket>>&,
		       const boost::system::error_code&);
    
    ~AsyncAcceptHandler() = default;
    void acceptHandle() const;
  };

  template <std::size_t N, std::size_t N2>
  AsyncAcceptHandler<N, N2>
  ::AsyncAcceptHandler(const std::shared_ptr
		       <Network::TCPBoostSocketServer<N, N2>>&
		       serverSocketPtr,
		       const std::shared_ptr
		       <Network::ITCPSocket
		       <boost::asio::ip::tcp::socket>>& socketPtr,
		       const boost::system::error_code& error) :
    _serverSocketPtr(serverSocketPtr),
    _socketPtr(socketPtr),
    _error(error)
  {
  }
  
  template <std::size_t N, std::size_t N2>
  void AsyncAcceptHandler<N, N2>::acceptHandle() const
  {
    if (!_error)
      {
	_socketPtr->recv();
	_serverSocketPtr->accept();
      }
  }
}

#endif /* !ASYNC_ACCEPT_HANDLER_HPP_ */
