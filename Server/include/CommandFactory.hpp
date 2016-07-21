#ifndef COMMAND_FACTORY_HPP_
# define COMMAND_FACTORY_HPP_

# include <string>
# include <map>
# include <utility>
# include <algorithm>
# include <iostream>
# include "CommandNotFoundException.hh"
# include "ICommand.hpp"

namespace Command
{
  template <typename T, typename U, typename V>
  class CommandFactory
  { 
  private :
    std::map<std::string, CommandPtr<T, U, V>>	_commands;

  public :
    CommandFactory() = default;
    ~CommandFactory() = default;
    void addCommand(const std::string&,
		    const CommandPtr<T, U, V>&);
    const CommandPtr<T, U, V>& getCommand(const std::string&);
    void listCommands() const;
  };

  template <typename T, typename U, typename V>
  void CommandFactory<T, U, V>
  ::addCommand(const std::string& name,
	       const CommandPtr<T, U, V>& commandPtr)
  {
    _commands[std::move(name)] = std::move(commandPtr);
  }

  template <typename T, typename U, typename V>
  const CommandPtr<T, U, V>& CommandFactory<T, U, V>
  ::getCommand(const std::string& name)
  {
    if (_commands.find(name) == _commands.cend())
      throw new Exception::Command
	::CommandNotFoundException("\"" + name + "\" command doesn't exist.");
    return _commands[std::move(name)];
  }

  template <typename T, typename U, typename V>
  void CommandFactory<T, U, V>::listCommands() const
  {
    std::cout << "Commands enabled :" << std::endl;
    std::for_each(_commands.cbegin(),
		  _commands.cend(),
		  [](const std::pair<std::string, CommandPtr<T, U, V>>& pair)
		  {
		    std::cout << pair.first << std::endl;
		  });
  }
}

#endif /* !COMMAND_FACTORY_HPP_ */
