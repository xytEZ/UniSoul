#ifndef COMMAND_EXECUTOR_HPP_
# define COMMAND_EXECUTOR_HPP_

# include "CommandNotFoundException.hh"
# include "ICommand.hpp"

namespace Command
{
  template <typename T, typename U, typename V>
  class CommandExecutor
  {
  private :
    CommandPtr<T, U, V>	_commandPtr;

  public :
    CommandExecutor();
    ~CommandExecutor() = default;
    void setCommandPtr(const CommandPtr<T, U, V>&);
    T execute(U&, const V&) const;
  };

  template <typename T, typename U, typename V>
  CommandExecutor<T, U, V>::CommandExecutor() : _commandPtr(nullptr) { }

  template <typename T, typename U, typename V>
  void CommandExecutor<T, U, V>
  ::setCommandPtr(const CommandPtr<T, U, V>& commandPtr)
  {
    _commandPtr = commandPtr;
  }

  template <typename T, typename U, typename V>
  T CommandExecutor<T, U, V>::execute(U& value, const V& value2) const
  {
    if (_commandPtr == nullptr)
      throw new Exception::Command
	::CommandNotFoundException("Command not initialized.");
    return _commandPtr->execute(value, value2);
  }
}

#endif /* !COMMAND_EXECUTOR_HPP_ */
