#ifndef REQUEST_PROCESSING_FROM_ASYNC_TASK_HANDLER_HPP_
# define REQUEST_PROCESSING_FROM_ASYNC_TASK_HANDLER_HPP_

# include <cstddef>
# include <memory>
# include <iterator>
# include <sstream>
# include <vector>
# include <string>
# include <algorithm>
# include <utility>
# include <boost/archive/archive_exception.hpp>

# include "SerializationTool.hpp"
# include "RequestExecuteFromAsyncTaskHandler.hpp"
# include "PacketSenderFromAsyncTaskHandler.hpp"
# include "DisconnectFromAsyncTaskHandler.hpp"
# include "ConnectionState.hh"

namespace Network
{
  template <typename T>
  class ITCPSocket;
  
  template <typename T, typename U, std::size_t N, std::size_t N2>
  class RequestProcessingFromAsyncTaskHandler
  {
  private :
    std::shared_ptr<Network::ITCPSocket
    <boost::asio::ip::tcp::socket>>		_socketPtr;

  public :
    RequestProcessingFromAsyncTaskHandler(const std::shared_ptr
					  <Network::ITCPSocket
					  <boost::asio::ip::tcp::socket>>&);
    
    ~RequestProcessingFromAsyncTaskHandler() = default;
    void requestProcessing() const;

  private :
    void validRequestProcessing(const T&, bool) const;
    void firstTimeValidRequestProcessing(const std::string&) const;
    void noValidRequestProcessing() const;
  };

  template <typename T, typename U, std::size_t N, std::size_t N2>
  RequestProcessingFromAsyncTaskHandler<T, U, N, N2>::
  RequestProcessingFromAsyncTaskHandler(const std::shared_ptr
					<Network::ITCPSocket
					<boost::asio::ip::tcp::socket>>&
					socketPtr) :
    _socketPtr(socketPtr)
  {
  }

  template <typename T, typename U, std::size_t N, std::size_t N2>
  void RequestProcessingFromAsyncTaskHandler<T, U, N, N2>
  ::requestProcessing() const
  {
    T	serializable;
    
    try
      {
	serializable = 
	  Serialization::Tool::deserialize<T>
	  (std::static_pointer_cast<Network::TCPBoostSocketServer<N, N2>>
	   (_socketPtr)->getBuffer());
	
	bool	registeredConnection =
	  !boost::any_cast
	  <typename Wrapper::UniSoulSystemWrapper::SocketManager&>
	  (std::static_pointer_cast<Network::TCPBoostSocketServer<N, N2>>
	   (_socketPtr)->getSystemWrapperPtrRef()
	   ->getContent()["SocketManager"])
	  .findSocketPtrIf([this]
			   (const std::shared_ptr
			    <Network::ISocket<boost::asio::ip::tcp::socket>>&
			    socketPtr) -> bool
			   {
			     return socketPtr == _socketPtr;
			   });
	
	validRequestProcessing(serializable, registeredConnection);
      }
    catch (const Exception::Serialization::SerializationFail&)
      {
	noValidRequestProcessing();
      }
    delete[] serializable.data;
  }

  template <typename T, typename U, std::size_t N, std::size_t N2>
  void RequestProcessingFromAsyncTaskHandler<T, U, N, N2>
  ::validRequestProcessing(const T& serializable, bool registeredConnection)
    const
  {
    std::vector<std::string>		datas;
    std::stringstream			ss;
    Network::ConnectionState		state;

    state = RequestExecuteFromAsyncTaskHandler<T, N, N2>
      (_socketPtr, serializable).requestExecute(datas);
    std::copy(datas.cbegin(),
	      datas.cend(),
	      std::ostream_iterator<std::string>(ss));
    PacketSenderFromAsyncTaskHandler<T, U, N, N2>
      (_socketPtr).successPacket(ss.str());
    if (state == Network::ConnectionState::DISCONNECTION
	|| state == Network::ConnectionState::REFUSED_CONNECTION)
      DisconnectFromAsyncTaskHandler<N, N2>(_socketPtr)
	.disconnect(registeredConnection);
    else if (registeredConnection)
      firstTimeValidRequestProcessing
	(serializable.data);
  }

  template <typename T, typename U, std::size_t N, std::size_t N2>
  void RequestProcessingFromAsyncTaskHandler<T, U, N, N2>
  ::firstTimeValidRequestProcessing(const std::string& dataFromRequest) const
  {
    const constexpr char			DELIMITER = ';';
    std::stringstream				ss(dataFromRequest);
    std::string					str;

    std::getline(ss, str, DELIMITER);
    _socketPtr->setRecipient(str);
    boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::SocketManager&>
      (std::static_pointer_cast<Network::TCPBoostSocketServer<N, N2>>
       (_socketPtr)->getSystemWrapperPtrRef()
       ->getContent()["SocketManager"]).addSocketPtr(_socketPtr);
  }
  
  template <typename T, typename U, std::size_t N, std::size_t N2>
  void RequestProcessingFromAsyncTaskHandler<T, U, N, N2>
  ::noValidRequestProcessing() const
  {
    PacketSenderFromAsyncTaskHandler<T, U, N, N2>(_socketPtr).errorPacket();
  }
}

#endif /* !REQUEST_PROCESSING_FROM_ASYNC_TASK_HANDLER_HPP_ */
