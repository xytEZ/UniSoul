#ifndef REQUEST_EXECUTE_FROM_ASYNC_TASK_HANDLER_HPP_
# define REQUEST_EXECUTE_FROM_ASYNC_TASK_HANDLER_HPP_

# include <cstddef>
# include <memory>
# include <string>

# include "UniSoulSystemWrapper.hh"
# include "UniSoulNetworkProtocol.hh"
# include "ConnectionStateFlag.hh"

namespace Network
{
  template <typename T>
  class ITCPSocket;
}

namespace Handler
{
  template <typename T, std::size_t N, std::size_t N2>
  class RequestExecuteFromAsyncTaskHandler
  {
  private :
    std::shared_ptr<Network::ITCPSocket
    <boost::asio::ip::tcp::socket>>			_socketPtr;
    
    std::shared_ptr<Serializable::ASerializable<T>>	_serializablePtr;

  public :
    RequestExecuteFromAsyncTaskHandler(const std::shared_ptr<Network::ITCPSocket
				       <boost::asio::ip::tcp::socket>>&,
				       const std::shared_ptr
				       <Serializable::ASerializable<T>>&);
    
    RequestExecuteFromAsyncTaskHandler() = default;
    Network::ConnectionStateFlag requestExecute(std::vector<std::string>&);
  };

  template <typename T, std::size_t N, std::size_t N2>
  RequestExecuteFromAsyncTaskHandler<T, N, N2>
  ::RequestExecuteFromAsyncTaskHandler(const std::shared_ptr<Network::ITCPSocket
				       <boost::asio::ip::tcp::socket>>& socketPtr,
				       const std::shared_ptr
				       <Serializable::ASerializable<T>>&
				       serializablePtr) :
    _socketPtr(socketPtr),
    _serializablePtr(serializablePtr)
  {
  }

  template <typename T, std::size_t N, std::size_t N2>
  Network::ConnectionStateFlag RequestExecuteFromAsyncTaskHandler<T, N, N2>
  ::requestExecute(std::vector<std::string>& datas)
  {
    std::string	dataFromPacket(_serializablePtr->getSerializableComponent()
			       .data);
    
    boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::CommandExecutor&>
      (std::static_pointer_cast<Network::TCPBoostSocketServer<N, N2>>
       (_socketPtr)->getSystemWrapperPtrRef()
       ->getContent()["CommandExecutor"])
      .setCommandPtr(boost::any_cast
		     <typename Wrapper::UniSoulSystemWrapper::CommandFactory&>
		     (std::static_pointer_cast
		      <Network::TCPBoostSocketServer<N, N2>>
		      (_socketPtr)->getSystemWrapperPtrRef()
		      ->getContent()["CommandFactory"])
		     .getCommand(static_cast<Command::Type>
				 (_serializablePtr->getSerializableComponent()
				  .command)));
    return boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::CommandExecutor&>
      (std::static_pointer_cast<Network::TCPBoostSocketServer<N, N2>>
       (_socketPtr)->getSystemWrapperPtrRef()
       ->getContent()["CommandExecutor"])
      .execute(std::static_pointer_cast<Network::TCPBoostSocketServer<N, N2>>
	       (_socketPtr)->getSystemWrapperPtrRef(), datas, dataFromPacket);
  }
}

#endif /* !REQUEST_EXECUTE_FROM_ASYNC_TASK_HANDLER_HPP_ */
