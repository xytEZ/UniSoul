#ifndef REQUEST_EXECUTE_FROM_ASYNC_TASK_HANDLER_HPP_
# define REQUEST_EXECUTE_FROM_ASYNC_TASK_HANDLER_HPP_

# include <cstddef>
# include <memory>
# include "UniSoulSystemWrapper.hh"
# include "RequestEnumConverter.hh"

namespace Network
{
  template <std::size_t N, std::size_t N2>
  class TCPBoostSocket;
}

namespace Handler
{
  template <std::size_t N, std::size_t N2>
  class RequestExecuteFromAsyncTaskHandler
  {
  private :
    const std::shared_ptr
    <Network::TCPBoostSocket<N, N2>>&			_socketPtr;
    const std::shared_ptr
    <Serializable::UniSoulNetworkProtocolSerializable>&	_serializablePtr;
    Network::Protocol::RequestEnumConverter		_requestEnumConverter;

  public :
    RequestExecuteFromAsyncTaskHandler(const std::shared_ptr
				       <Network::TCPBoostSocket<N, N2>>&,
				       const std::shared_ptr
				       <Serializable::
				       UniSoulNetworkProtocolSerializable>&);
    RequestExecuteFromAsyncTaskHandler() = default;
    bool requestExecute() const;
  };

  template <std::size_t N, std::size_t N2>
  RequestExecuteFromAsyncTaskHandler<N, N2>
  ::RequestExecuteFromAsyncTaskHandler(const std::shared_ptr
				       <Network::TCPBoostSocket<N, N2>>&
				       socketPtr,
				       const std::shared_ptr
				       <Serializable::
				       UniSoulNetworkProtocolSerializable>&
				       serializablePtr) :
    _socketPtr(socketPtr),
    _serializablePtr(serializablePtr)
  {
  }

  template <std::size_t N, std::size_t N2>
  bool RequestExecuteFromAsyncTaskHandler<N, N2>::requestExecute() const
  {
    boost::any_cast
      <typename UniSoulSystemWrapper::CommandExecutor&>
      (_socketPtr->getSystemWrapperPtrRef()
       ->getContent()["CommandExecutor"])
      .setCommandPtr(boost::any_cast
		     <typename UniSoulSystemWrapper::CommandFactory&>
		     (_socketPtr->getSystemWrapperPtrRef()
		      ->getContent()["CommandFactory"])
		     .getCommand(_requestEnumConverter.convertRequestToString
				 (_serializablePtr->getSerializableComponent()
				  .header.request)));
    /*boost::any_cast
      <typename UniSoulSystemWrapper::CommandExecutor&>
      (_socketPtr->getSystemWrapperPtrRef()
       ->getContent()["CommandExecutor"])
       .execute();*/
    return true;
  }
}

#endif /* !REQUEST_EXECUTE_FROM_ASYNC_TASK_HANDLER_HPP_ */
