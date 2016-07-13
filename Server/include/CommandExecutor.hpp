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
    std::shared_ptr<ICommand<T>>	_command;

  public :
    CommandExecutor();
    ~CommandExecutor();
    void setCommand(const std::shared_ptr<ICommand<T>>&);
    bool execute(T&) const;
  };

  template <typename T>
  CommandExecutor<T>::CommandExecutor() : _command(nullptr) { }

  template <typename T>
  CommandExecutor<T>::~CommandExecutor() { }

  template <typename T>
  void CommandExecutor<T>::setCommand(const std::shared_ptr<ICommand<T>>& command)
  {
    _command = command;
  }

  template <typename T>
  bool CommandExecutor<T>::execute(T& value) const
  {
    return _command->execute(value);
  }
}

#endif /* !COMMAND_EXECUTOR_HPP_ */
