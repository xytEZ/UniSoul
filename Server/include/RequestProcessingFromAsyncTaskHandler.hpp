
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
# include "NetworkPacketException.hh"
# include "RequestExecuteFromAsyncTaskHandler.hpp"
# include "PacketSenderFromAsyncTaskHandler.hpp"
# include "DisconnectFromAsyncTaskHandler.hpp"

namespace Network
{
  template <std::size_t N, std::size_t N2>
  class TCPBoostSocket;
}

namespace Handler
{
  template <typename T, typename U, typename V, std::size_t N, std::size_t N2>
  class RequestProcessingFromAsyncTaskHandler
  {
  private :
    const std::shared_ptr
    <Network::TCPBoostSocket<N, N2>>&	_socketPtr;
    SerializationHandler<T>		_serializationHandler;

  public :
    RequestProcessingFromAsyncTaskHandler(const std::shared_ptr
					  <Network::TCPBoostSocket<N, N2>>&);
    ~RequestProcessingFromAsyncTaskHandler() = default;
    void requestProcessing() const;

  private :
    void validRequestProcessing(const std::shared_ptr
				<Serializable::ASerializable<T>>&,
				bool) const;
    
    template <typename W>
    void firstTimeValidRequestProcessing(const std::string&) const;
    
    void noValidRequestProcessing(bool) const;
  };

  template <typename T, typename U, typename V, std::size_t N, std::size_t N2>
  RequestProcessingFromAsyncTaskHandler<T, U, V, N, N2>::
  RequestProcessingFromAsyncTaskHandler(const std::shared_ptr
					<Network::TCPBoostSocket<N, N2>>&
					socketPtr) :
    _socketPtr(socketPtr)
  {
  }

  template <typename T, typename U, typename V, std::size_t N, std::size_t N2>
  void RequestProcessingFromAsyncTaskHandler<T, U, V, N, N2>
  ::requestProcessing() const
  {
    bool isFirstRequestFromConnection =
      boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::ConnectionManager&>
      (_socketPtr->getSystemWrapperPtrRef()->getContent()["ConnectionManager"])
      .getConnectionIf([this](const std::shared_ptr
				     <Network::TCPConnection
				     <Info::ClientInfo>>&
				     connectionPtr) -> bool
		       {
			 return connectionPtr->getSocketPtr() == _socketPtr;
		       });
    
    try
      {
	std::shared_ptr<Serializable::ASerializable<T>>	serializablePtr
	  = _serializationHandler.deserialize(_socketPtr->getBuffer());

	validRequestProcessing(serializablePtr, isFirstRequestFromConnection);
      }
    catch (const Exception::SerializationException&)
      {
	noValidRequestProcessing(isFirstRequestFromConnection);
      }
  }

  template <typename T, typename U, typename V, std::size_t N, std::size_t N2>
  void RequestProcessingFromAsyncTaskHandler<T, U, V, N, N2>
  ::validRequestProcessing(const std::shared_ptr
			   <Serializable::ASerializable<T>>&
			   serializablePtr,
			   bool isFirstRequestFromConnection) const
  {
    std::vector<std::string>        datas;
    std::stringstream               ss;
    bool			    ret;

    ret = RequestExecuteFromAsyncTaskHandler<T, N, N2>
      (_socketPtr, serializablePtr).requestExecute(datas);
    std::copy(datas.cbegin(),
	      datas.cend(),
	      std::ostream_iterator<std::string>(ss, "|"));
    PacketSenderFromAsyncTaskHandler<T, U, V, N, N2>
      (_socketPtr, _serializationHandler).packetSuccess(ss.str());
    if (ret)
      DisconnectFromAsyncTaskHandler<N, N2>(_socketPtr).disconnect();
    else if (isFirstRequestFromConnection)
      firstTimeValidRequestProcessing<Info::ClientInfo>
	(serializablePtr->getSerializableComponent().data);
  }

  template <typename T, typename U, typename V, std::size_t N, std::size_t N2>
  template <typename W>
  void RequestProcessingFromAsyncTaskHandler<T, U, V, N, N2>
  ::firstTimeValidRequestProcessing(const std::string& dataFromRequest) const
  {
    constexpr const char			DELIMITER = ';';
    std::shared_ptr<Network::TCPConnection<W>>	connectionPtr =
      std::make_shared<Network::TCPConnection<W>>(_socketPtr);
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
      (_socketPtr->getSystemWrapperPtrRef()->getContent()["ConnectionManager"])
      .addConnection(connectionPtr);
  }
  
  template <typename T, typename U, typename V, std::size_t N, std::size_t N2>
  void RequestProcessingFromAsyncTaskHandler<T, U, V, N, N2>
  ::noValidRequestProcessing(bool isFirstRequestFromConnection) const
  {
    if (isFirstRequestFromConnection)
      _socketPtr->setMaintainInstance(false);
    PacketSenderFromAsyncTaskHandler<T, U, V, N, N2>
      (_socketPtr, _serializationHandler).packetError();
  }
}

#endif /* !REQUEST_PROCESSING_FROM_ASYNC_TASK_HANDLER_HPP_ */
