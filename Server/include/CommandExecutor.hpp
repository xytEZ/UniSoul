#ifndef COMMAND_EXECUTOR_HPP_
# define COMMAND_EXECUTOR_HPP_

# include <memory>

namespace Command
{
  template <typename T>
  class ICommand;
  
  template <typename T>
  class CommandExecutor
  {
  private :
    std::shared_ptr<ICommand<T>>	_commandPtr;

  public :
    CommandExecutor();
    ~CommandExecutor();
    void setCommandPtr(const std::shared_ptr<ICommand<T>>&);
    bool execute(T&) const;
  };

  template <typename T>
  CommandExecutor<T>::CommandExecutor() : _commandPtr(nullptr) { }

  template <typename T>
  CommandExecutor<T>::~CommandExecutor() { }

  template <typename T>
  void CommandExecutor<T>::setCommandPtr(const std::shared_ptr<ICommand<T>>& commandPtr)
  {
    _commandPtr = commandPtr;
  }

  template <typename T>
  bool CommandExecutor<T>::execute(T& value) const
  {
    return _commandPtr->execute(value);
  }
}

#endif /* !COMMAND_EXECUTOR_HPP_ */
