#ifndef ASYNC_WRITE_HANDLER_HPP_
# define ASYNC_WRITE_HANDLER_HPP_

# include <memory>
# include <boost/system/error_code.hpp>
# include "DisconnectFromAsyncTaskHandler.hpp"
# include "BoostDescriptor.hh"

namespace Network
{
  template <std::size_t N, std::size_t N2>
  class TCPBoostSocket;
}

namespace Handler
{
  template <std::size_t N, std::size_t N2>
  class AsyncWriteHandler
  {
  private :
    const std::shared_ptr
    <Network::TCPBoostSocket<N, N2>>&	_socketPtr;
    const boost::system::error_code&	_error;
    
  public :
    AsyncWriteHandler(const std::shared_ptr<Network::TCPBoostSocket<N, N2>>&,
		      const boost::system::error_code&);
    ~AsyncWriteHandler() = default;
    void writeHandle() const;
  };

  template <std::size_t N, std::size_t N2>
  AsyncWriteHandler<N, N2>
  ::AsyncWriteHandler(const std::shared_ptr
		      <Network::TCPBoostSocket<N, N2>>& socketPtr,
		      const boost::system::error_code& error) :
    _socketPtr(socketPtr),
    _error(error)
  {
  }
  
  template <std::size_t N, std::size_t N2>
  void AsyncWriteHandler<N, N2>::writeHandle() const
  {
    bool	registeredConnection =
      boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::ConnectionManager&>
      (_socketPtr->getSystemWrapperPtrRef()
       ->getContent()["ConnectionManager"])
      .findConnectionIf([this](const std::shared_ptr
			       <Network::TCPConnection
			       <Info::ClientInfo, ::Descriptor>>&
			       connectionPtr) -> bool
			{
			  return connectionPtr->getSocketPtr() == _socketPtr;
			});
    if (_error || !registeredConnection)
      DisconnectFromAsyncTaskHandler<N, N2>(_socketPtr)
	.disconnect(registeredConnection);
  }
}

#endif /* !ASYNC_WRITE_HANDLER_HPP_ */