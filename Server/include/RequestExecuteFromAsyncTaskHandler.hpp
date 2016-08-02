#ifndef REQUEST_EXECUTE_FROM_ASYNC_TASK_HANDLER_HPP_
# define REQUEST_EXECUTE_FROM_ASYNC_TASK_HANDLER_HPP_

# include <cstddef>
# include <memory>
# include <string>
# include "UniSoulSystemWrapper.hh"
# include "UniSoulNetworkProtocol.hh"
# include "BoostDescriptor.hh"

namespace Network
{
  template <std::size_t N, std::size_t N2>
  class TCPBoostSocket;
}

namespace Handler
{
  template <typename T, std::size_t N, std::size_t N2>
  class RequestExecuteFromAsyncTaskHandler
  {
  private :
    const std::shared_ptr
    <Network::TCPBoostSocket<N, N2>>&	_socketPtr;
    const std::shared_ptr
    <Serializable::ASerializable<T>>&	_serializablePtr;

  public :
    RequestExecuteFromAsyncTaskHandler(const std::shared_ptr
				       <Network::TCPBoostSocket<N, N2>>&,
				       const std::shared_ptr
				       <Serializable::ASerializable<T>>&);
    RequestExecuteFromAsyncTaskHandler() = default;
    bool requestExecute(std::vector<std::string>&);
  };

  template <typename T, std::size_t N, std::size_t N2>
  RequestExecuteFromAsyncTaskHandler<T, N, N2>
  ::RequestExecuteFromAsyncTaskHandler(const std::shared_ptr
				       <Network::TCPBoostSocket<N, N2>>&
				       socketPtr,
				       const std::shared_ptr
				       <Serializable::ASerializable<T>>&
				       serializablePtr) :
    _socketPtr(socketPtr),
    _serializablePtr(serializablePtr)
  {
  }

  template <typename T, std::size_t N, std::size_t N2>
  bool RequestExecuteFromAsyncTaskHandler<T, N, N2>
  ::requestExecute(std::vector<std::string>& datas)
  {
    std::string	dataFromPacket(_serializablePtr->getSerializableComponent()
			       .data);
    
    boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::CommandExecutor&>
      (_socketPtr->getSystemWrapperPtrRef()
       ->getContent()["CommandExecutor"])
      .setCommandPtr(boost::any_cast
		     <typename Wrapper::UniSoulSystemWrapper::CommandFactory&>
		     (_socketPtr->getSystemWrapperPtrRef()
		      ->getContent()["CommandFactory"])
		     .getCommand(static_cast<Command::Command>
				 (_serializablePtr->getSerializableComponent()
				  .command)));
    return boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::CommandExecutor&>
      (_socketPtr->getSystemWrapperPtrRef()
       ->getContent()["CommandExecutor"])
      .execute(_socketPtr->getSystemWrapperPtrRef(), datas, dataFromPacket);
  }
}

#endif /* !REQUEST_EXECUTE_FROM_ASYNC_TASK_HANDLER_HPP_ */
