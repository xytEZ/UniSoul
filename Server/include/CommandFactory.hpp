#ifndef COMMAND_FACTORY_HPP_
# define COMMAND_FACTORY_HPP_

# include <string>
# include <map>
# include <utility>
# include <algorithm>
# include <iostream>
# include "CommandNotFoundException.hh"
# include "ConnectionCommand.hpp"
# include "DeconnectionCommand.hpp"

namespace Command
{ 
  template <typename T, typename... Args>
  class CommandFactory
  { 
  private :
    std::map<std::string, CommandPtr<T, Args...>>	_commands;

  public :
    CommandFactory();
    ~CommandFactory() = default;
    void addCommand(const std::string&,
		    const CommandPtr<T, Args...>&);
    const CommandPtr<T, Args...>& getCommand(const std::string&);
    void listCommands() const;
  };

  template <typename T, typename... Args>
  CommandFactory<T, Args...>::CommandFactory() :
    _commands
  {
    { "Connection", std::make_shared<ConnectionCommand<T, Args...>>() },
      { "Deconnection", std::make_shared<DeconnectionCommand<T, Args...>>() }
  }
    {
    }
  
  template <typename T, typename... Args>
  void CommandFactory<T, Args...>
  ::addCommand(const std::string& name,
	       const CommandPtr<T, Args...>& commandPtr)
  {
    _commands[std::move(name)] = std::move(commandPtr);
  }

  template <typename T, typename... Args>
  const CommandPtr<T, Args...>& CommandFactory<T, Args...>
  ::getCommand(const std::string& name)
  {
    if (_commands.find(name) == _commands.cend())
      throw new Exception::Command
	::CommandNotFoundException("\"" + name + "\" command doesn't exist.");
    return _commands[std::move(name)];
  }
  
  template <typename T, typename... Args>
  void CommandFactory<T, Args...>::listCommands() const
  {
    std::cout << "Commands enabled :" << std::endl;
    std::for_each(_commands.cbegin(),
		  _commands.cend(),
		  [](const std::pair
		     <std::string, CommandPtr<T, Args...>>& pair)
		  {
		    std::cout << pair.first << std::endl;
		  });
  }
}

#endif /* !COMMAND_FACTORY_HPP_ */
