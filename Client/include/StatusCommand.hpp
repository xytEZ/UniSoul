#ifndef STATUS_COMMAND_HPP_
# define STATUS_COMMAND_HPP_

# include <tuple>

# include "ICommand.hpp"

namespace Command
{
  template <typename T, typename... Args>
  class StatusCommand : public ICommand<T, Args...>
  {
  public :
    StatusCommand() = default;
    virtual ~StatusCommand() = default;
    virtual T execute(Args&...) const;
  };

  template <typename T, typename... Args>
  T StatusCommand<T, Args...>::execute(Args&... args) const
  {
    std::tuple<Args&...>	tuple(std::forward_as_tuple(args...));
    
    if (std::get<0>(tuple)->getSocketCallbacksPtr().size())
      {
	std::get<5>(tuple)
	  .append("Established connection with :");
	std::for_each(std::get<0>(tuple)->getSocketCallbacksPtr().cbegin(),
		      std::get<0>(tuple)->getSocketCallbacksPtr().cend(),
		      [&tuple]
		      (const Network::IMultiplexer::SocketCallbackPtr&
		       socketCallbackPtr) -> void
		      {
			std::shared_ptr
			  <Network::ITCPSocket<int>>	tcpSocketPtr =
			  std::dynamic_pointer_cast<Network::ITCPSocket<int>>
			  (socketCallbackPtr->socketPtr);
			
			if (tcpSocketPtr)
			  {
			    std::get<5>(tuple)
			      .append("\n\t- \"")
			      .append(tcpSocketPtr->getRecipient())
			      .append("\" (")
			      .append(tcpSocketPtr->getAddress())
			      .append(":")
			      .append(std::to_string(tcpSocketPtr->getPort()))
			      .append(")");
			  }
		      });
      }
    else
      std::get<5>(tuple)
	.append("Awaiting the establishment of a connection...");
    return App::State::Flag::RUNNING;
  }
}

#endif /* !STATUS_COMMAND_HPP_ */
