#ifndef COMMAND_FACTORY_HPP_
# define COMMAND_FACTORY_HPP_

# include <memory>
# include <string>
# include <map>
# include <utility>
# include <algorithm>
# include <iostream>

namespace Command
{
  template <typename T>
  class ICommand;

  template <typename T>
  class CommandFactory
  { 
  private :
    std::map<std::string, std::shared_ptr<ICommand<T>>>	_commands;

  public :
    CommandFactory();
    ~CommandFactory();
    void addCommand(const std::string&,
		    const std::shared_ptr<ICommand<T>>&);
    std::shared_ptr<Command::ICommand<T>>& getCommand(const std::string&);
    void listCommands() const;
  };

  template <typename T>
  CommandFactory<T>::CommandFactory() : _commands() { }

  template <typename T>
  CommandFactory<T>::~CommandFactory() { }

  template <typename T>
  void CommandFactory<T>::addCommand(const std::string& name,
				     const std::shared_ptr<ICommand<T>>& command)
  {
    _commands[std::move(name)] = std::move(command);
  }

  template <typename T>
  std::shared_ptr<ICommand<T>>& CommandFactory<T>::getCommand(const std::string& name)
  {
    return _commands[std::move(name)];
  }

  template <typename T>
  void CommandFactory<T>::listCommands() const
  {
    std::cout << "Commands enabled :" << std::endl;
    std::for_each(_commands.cbegin(),
		  _commands.cend(),
		  [](const std::pair<std::string, std::shared_ptr<ICommand<T>>>& pair)
		  {
		    std::cout << pair.first << std::endl;
		  });
  }
}

#endif /* !COMMAND_FACTORY_HPP_ */
