#ifndef COMMAND_EXECUTOR_HPP_
# define COMMAND_EXECUTOR_HPP_

# include "CommandNotFoundException.hh"
# include "ICommand.hpp"

namespace Command
{
  template <typename T, typename... Args>
  class CommandExecutor
  {
  private :
    CommandPtr<T, Args...>	_commandPtr;

  public :
    CommandExecutor();
    ~CommandExecutor() = default;
    void setCommandPtr(const CommandPtr<T, Args...>&);
    T execute(Args&...) const;
  };

  template <typename T, typename... Args>
  CommandExecutor<T, Args...>::CommandExecutor() : _commandPtr(nullptr) { }

  template <typename T, typename... Args>
  void CommandExecutor<T, Args...>
  ::setCommandPtr(const CommandPtr<T, Args...>& commandPtr)
  {
    _commandPtr = commandPtr;
  }

  template <typename T, typename... Args>
  T CommandExecutor<T, Args...>::execute(Args&... args) const
  {
    if (_commandPtr == nullptr)
      throw new Exception::Command
	::CommandNotFoundException("Unknown command.");
    return _commandPtr->execute(args...);
  }
}

#endif /* !COMMAND_EXECUTOR_HPP_ */
