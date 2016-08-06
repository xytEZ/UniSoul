#ifndef REQUEST_PROCESSING_FROM_ASYNC_TASK_HANDLER_HPP_
# define REQUEST_PROCESSING_FROM_ASYNC_TASK_HANDLER_HPP_

# include <cstddef>
# include <memory>
# include <iterator>
# include <sstream>
# include <vector>
# include <string>
# include <algorithm>
# include <boost/archive/archive_exception.hpp>

# include "SerializationHandler.hpp"
# include "RequestExecuteFromAsyncTaskHandler.hpp"
# include "PacketSenderFromAsyncTaskHandler.hpp"
# include "DisconnectFromAsyncTaskHandler.hpp"
# include "ConnectionStateFlag.hh"

namespace Network
{
  template <typename T>
  class ITCPSocket;
}

namespace Handler
{
  template <typename T, typename U, typename V, std::size_t N, std::size_t N2>
  class RequestProcessingFromAsyncTaskHandler
  {
  private :
    std::shared_ptr<Network::ITCPSocket
    <boost::asio::ip::tcp::socket>>	_socketPtr;
    
    SerializationHandler<T>		_serializationHandler;

  public :
    RequestProcessingFromAsyncTaskHandler(const std::shared_ptr
					  <Network::ITCPSocket
					  <boost::asio::ip::tcp::socket>>&);
    
    ~RequestProcessingFromAsyncTaskHandler() = default;
    void requestProcessing() const;

  private :
    void validRequestProcessing(const std::shared_ptr
				<Serializable::ASerializable<T>>&,
				bool) const;
    
    template <typename W>
    void firstTimeValidRequestProcessing(const std::string&) const;
    
    void noValidRequestProcessing() const;
  };

  template <typename T, typename U, typename V, std::size_t N, std::size_t N2>
  RequestProcessingFromAsyncTaskHandler<T, U, V, N, N2>::
  RequestProcessingFromAsyncTaskHandler(const std::shared_ptr
					<Network::ITCPSocket
					<boost::asio::ip::tcp::socket>>&
					socketPtr) :
    _socketPtr(socketPtr),
    _serializationHandler()
  {
  }

  template <typename T, typename U, typename V, std::size_t N, std::size_t N2>
  void RequestProcessingFromAsyncTaskHandler<T, U, V, N, N2>
  ::requestProcessing() const
  {
    
    try
      {
	std::shared_ptr<Serializable::ASerializable<T>>	serializablePtr =
	  _serializationHandler.deserialize
	  (std::static_pointer_cast<Network::TCPBoostSocketServer<N, N2>>
	   (_socketPtr)->getBuffer());
	bool						registeredConnection =
	  boost::any_cast
	  <typename Wrapper::UniSoulSystemWrapper::ConnectionManager&>
	  (std::static_pointer_cast<Network::TCPBoostSocketServer<N, N2>>
	   (_socketPtr)->getSystemWrapperPtrRef()
	   ->getContent()["ConnectionManager"])
	  .findConnectionIf([this](const std::shared_ptr
				   <Network::TCPConnection
				   <Info::ClientInfo,
				   boost::asio::ip::tcp::socket>>&
				   connectionPtr) -> bool
			    {
			      return connectionPtr
				->getSocketPtr() == _socketPtr;
			    });
	
	validRequestProcessing(serializablePtr, registeredConnection);
      }
    catch (const Exception::SerializationException&)
      {
	noValidRequestProcessing();
      }
  }

  template <typename T, typename U, typename V, std::size_t N, std::size_t N2>
  void RequestProcessingFromAsyncTaskHandler<T, U, V, N, N2>
  ::validRequestProcessing(const std::shared_ptr
			   <Serializable::ASerializable<T>>&
			   serializablePtr,
			   bool registeredConnection) const
  {
    std::vector<std::string>		datas;
    std::stringstream			ss;
    Network::ConnectionStateFlag	state;

    state = RequestExecuteFromAsyncTaskHandler<T, N, N2>
      (_socketPtr, serializablePtr).requestExecute(datas);
    std::copy(datas.cbegin(),
	      datas.cend(),
	      std::ostream_iterator<std::string>(ss, "|"));
    PacketSenderFromAsyncTaskHandler<T, U, V, N, N2>
      (_socketPtr, _serializationHandler).packetSuccess(ss.str());
    if (state)
      DisconnectFromAsyncTaskHandler<N, N2>(_socketPtr)
	.disconnect(registeredConnection);
    else if (registeredConnection)
      firstTimeValidRequestProcessing<Info::ClientInfo>
	(serializablePtr->getSerializableComponent().data);
  }

  template <typename T, typename U, typename V, std::size_t N, std::size_t N2>
  template <typename W>
  void RequestProcessingFromAsyncTaskHandler<T, U, V, N, N2>
  ::firstTimeValidRequestProcessing(const std::string& dataFromRequest) const
  {
    const constexpr char			DELIMITER = ';';
    
    std::shared_ptr<Network::TCPConnection
       <W, boost::asio::ip::tcp::socket>>	connectionPtr =
      std::make_shared<Network::TCPConnection
		       <W, boost::asio::ip::tcp::socket>>(_socketPtr);
    
    std::stringstream				ss(dataFromRequest);
    std::string					str;
    
    std::getline(ss, str, DELIMITER);
    connectionPtr->getClientInfo().firstName = str;
    std::getline(ss, str, DELIMITER);
    connectionPtr->getClientInfo().lastName = str;
    std::getline(ss, str, DELIMITER);
    connectionPtr->getClientInfo().email = str;
    boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::ConnectionManager&>
      (std::static_pointer_cast<Network::TCPBoostSocketServer<N, N2>>
       (_socketPtr)->getSystemWrapperPtrRef()->getContent()["ConnectionManager"])
      .addConnection(connectionPtr);
  }
  
  template <typename T, typename U, typename V, std::size_t N, std::size_t N2>
  void RequestProcessingFromAsyncTaskHandler<T, U, V, N, N2>
  ::noValidRequestProcessing() const
  {
    PacketSenderFromAsyncTaskHandler<T, U, V, N, N2>
      (_socketPtr, _serializationHandler).packetError();
  }
}

#endif /* !REQUEST_PROCESSING_FROM_ASYNC_TASK_HANDLER_HPP_ */
