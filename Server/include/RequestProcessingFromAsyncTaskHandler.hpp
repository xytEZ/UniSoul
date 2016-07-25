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
# include "IPacketStateChecker.hpp"
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
  template <typename T, typename U, typename V, typename W, std::size_t N,
	    std::size_t N2>
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
    void requestProcessing(bool) const;

  private :
    void validRequestProcessing(const std::shared_ptr
				<Serializable::ASerializable<T>>&,
				bool) const;
    
    template <typename X>
    void firstTimeValidRequestProcessing(const std::string&) const;
    
    void noValidRequestProcessing(bool) const;
  };

  template <typename T, typename U, typename V, typename W, std::size_t N,
	    std::size_t N2>
  RequestProcessingFromAsyncTaskHandler<T, U, V, W, N, N2>::
  RequestProcessingFromAsyncTaskHandler(const std::shared_ptr
					<Network::TCPBoostSocket<N, N2>>&
					socketPtr) :
    _socketPtr(socketPtr)
  {
  }

  template <typename T, typename U, typename V, typename W, std::size_t N,
	    std::size_t N2>
  void RequestProcessingFromAsyncTaskHandler<T, U, V, W, N, N2>
  ::requestProcessing(bool isFirstTime) const
  {
    try
      {
	std::unique_ptr
	  <Network::IPacketStateChecker<T>>	packetStateChecker =
	  std::make_unique<W>();
	std::shared_ptr<Serializable::ASerializable<T>>	serializablePtr
	  = _serializationHandler.deserialize(_socketPtr->getBuffer());

	packetStateChecker->setPacket(serializablePtr
				      ->getSerializableComponent());
	packetStateChecker->checkPacket();
	validRequestProcessing(serializablePtr, isFirstTime);
      }
    catch (const boost::archive::archive_exception&)
      {
	noValidRequestProcessing(isFirstTime);
      }
    catch (const Exception::Network::PacketStateException&)
      {
	noValidRequestProcessing(isFirstTime);
      }
  }

  template <typename T, typename U, typename V, typename W, std::size_t N,
	    std::size_t N2>
  void RequestProcessingFromAsyncTaskHandler<T, U, V, W, N, N2>
  ::validRequestProcessing(const std::shared_ptr
			   <Serializable::ASerializable<T>>&
			   serializablePtr,
			   bool isFirstTime) const
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
    else if (isFirstTime)
      firstTimeValidRequestProcessing<Info::ClientInfo>
	(reinterpret_cast<const char *>(serializablePtr
					->getSerializableComponent()
					.data.data));
  }

  template <typename T, typename U, typename V, typename W, std::size_t N,
	    std::size_t N2>
  template <typename X>
  void RequestProcessingFromAsyncTaskHandler<T, U, V, W, N, N2>
  ::firstTimeValidRequestProcessing(const std::string& dataFromRequest) const
  {
    constexpr const char			DELIMITER = ';';
    std::shared_ptr<Network::TCPConnection<X>>	connectionPtr =
      std::make_shared<Network::TCPConnection<X>>(_socketPtr);
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
      (_socketPtr->getSystemWrapperPtrRef()
       ->getContent()["ConnectionManager"])
      .addConnection(connectionPtr);
  }
  
  template <typename T, typename U, typename V, typename W, std::size_t N,
	    std::size_t N2>
  void RequestProcessingFromAsyncTaskHandler<T, U, V, W, N, N2>
  ::noValidRequestProcessing(bool isFirstTime) const
  {
    PacketSenderFromAsyncTaskHandler<T, U, V, N, N2>
      (_socketPtr, _serializationHandler).packetError(isFirstTime);
  }
}

#endif /* !REQUEST_PROCESSING_FROM_ASYNC_TASK_HANDLER_HPP_ */
