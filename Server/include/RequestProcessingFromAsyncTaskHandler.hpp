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

# include "ComplementaryFunction.hpp"
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
    bool requestProcessing() const;

  private :
    bool validRequestProcessing(const T&, bool) const;
    bool firstTimeValidRequestProcessing(const std::string&) const;
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
  bool
  RequestProcessingFromAsyncTaskHandler<T, U, N, N2>::requestProcessing() const
  {
    T		serializable;
    bool	socketIsClosed = false;
    
    try
      {
	bool	registeredConnection;
	
	serializable = 
	  Serialization::Tool::deserialize<T>
	  (std::static_pointer_cast<Network::TCPBoostSocketServer<N, N2>>
	   (_socketPtr)->getBuffer());
	registeredConnection =
	  static_cast<bool>
	  (boost::any_cast
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
			   }));
	socketIsClosed =
	  validRequestProcessing(serializable, registeredConnection);
      }
    catch (const Exception::Serialization::SerializationFail&)
      {
	noValidRequestProcessing();
      }
    delete[] serializable.data;
    return socketIsClosed;
  }

  template <typename T, typename U, std::size_t N, std::size_t N2>
  bool RequestProcessingFromAsyncTaskHandler<T, U, N, N2>
  ::validRequestProcessing(const T& serializable, bool registeredConnection)
    const
  {
    std::string			retMsg;
    Network::ConnectionState	state;
    bool			socketIsClosed = false;

    state = RequestExecuteFromAsyncTaskHandler<T, N, N2>
      (_socketPtr, serializable).requestExecute(retMsg);
    PacketSenderFromAsyncTaskHandler<T, U, N, N2>
      (_socketPtr).successPacket(retMsg);
    if (state == Network::ConnectionState::DISCONNECTION
	|| state == Network::ConnectionState::REFUSED_CONNECTION)
      socketIsClosed = DisconnectFromAsyncTaskHandler<N, N2>(_socketPtr)
	.disconnect(registeredConnection);
    else if (!registeredConnection)
      socketIsClosed = firstTimeValidRequestProcessing(serializable.data);
    return socketIsClosed;
  }

  template <typename T, typename U, std::size_t N, std::size_t N2>
  bool RequestProcessingFromAsyncTaskHandler<T, U, N, N2>
  ::firstTimeValidRequestProcessing(const std::string& dataFromRequest) const
  {
    constexpr const char	DELIMITER = ';';
    RemoteConnectionInfo	remoteConnectionInfo;
    std::stringstream		ss(dataFromRequest);
    std::string			s;

    std::getline(ss, s, DELIMITER);
    remoteConnectionInfo.login = std::move(s);
    ss.str(dataFromRequest.substr(dataFromRequest.find('|') + 1));
    std::getline(ss, s, DELIMITER);
    remoteConnectionInfo.listeningAddress = std::move(s);
    std::getline(ss, s, DELIMITER);
    remoteConnectionInfo.listeningPort =
      Tool::convert_string_to<unsigned short>(std::move(s));
    _socketPtr->setRemoteConnectionInfo(std::move(remoteConnectionInfo));
    boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::SocketManager&>
      (std::static_pointer_cast<Network::TCPBoostSocketServer<N, N2>>
       (_socketPtr)->getSystemWrapperPtrRef()
       ->getContent()["SocketManager"]).addSocketPtr(_socketPtr);
    return !true;
  }
  
  template <typename T, typename U, std::size_t N, std::size_t N2>
  void RequestProcessingFromAsyncTaskHandler<T, U, N, N2>
  ::noValidRequestProcessing() const
  {
    PacketSenderFromAsyncTaskHandler<T, U, N, N2>(_socketPtr).errorPacket();
  }
}

#endif /* !REQUEST_PROCESSING_FROM_ASYNC_TASK_HANDLER_HPP_ */
