#ifndef ASYNC_WRITE_HANDLER_HPP_
# define ASYNC_WRITE_HANDLER_HPP_

# include <memory>
# include <boost/system/error_code.hpp>
# include "DisconnectFromAsyncTaskHandler.hpp"

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

  private :
    void firstTimeWriteHandle() const;
    void regularWriteHandle() const;
    void lastTimeWriteHandle() const;
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
    static bool	isFirstTime = true;
    
    if (!_error)
      {
	if (isFirstTime)
	  {
	    firstTimeWriteHandle();
	    isFirstTime = false;
	  }
	else
	  regularWriteHandle();
      }
    lastTimeWriteHandle();
  }

  template <std::size_t N, std::size_t N2>
  void AsyncWriteHandler<N, N2>::firstTimeWriteHandle() const
  {
  }

  template <std::size_t N, std::size_t N2>
  void AsyncWriteHandler<N, N2>::regularWriteHandle() const
  {
  }

  template <std::size_t N, std::size_t N2>
  void AsyncWriteHandler<N, N2>::lastTimeWriteHandle() const
  {
    Handler::DisconnectFromAsyncTaskHandler<N, N2>(_socketPtr).disconnect();
  }
}

#endif /* !ASYNC_WRITE_HANDLER_HPP_ */
