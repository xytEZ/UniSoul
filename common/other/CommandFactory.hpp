#ifndef COMMAND_FACTORY_HPP_
# define COMMAND_FACTORY_HPP_

# include <map>
# include <utility>
# include <algorithm>
# include <iostream>
# include "UnknownCommandException.hh"
# include "ICommand.hpp"

namespace Command
{ 
  template <typename T, typename U, typename... Args>
  class CommandFactory
  { 
  private :
    std::map<T, CommandPtr<U, Args...>>	_commands;

  public :
    CommandFactory() = default;
    ~CommandFactory() = default;
    void addCommand(const T&,
		    const CommandPtr<U, Args...>&);
    const CommandPtr<U, Args...>& getCommand(const T&);
    void listCommands() const;
  };
  
  template <typename T, typename U, typename... Args>
  void CommandFactory<T, U, Args...>
  ::addCommand(const T& key,
	       const CommandPtr<U, Args...>& commandPtr)
  {
    _commands[std::move(key)] = std::move(commandPtr);
  }

  template <typename T, typename U, typename... Args>
  const CommandPtr<U, Args...>& CommandFactory<T, U, Args...>
  ::getCommand(const T& key)
  {
    if (_commands.find(key) == _commands.cend())
      throw new Exception::Command::UnknownCommand("Unknown command");
    return _commands[std::move(key)];
  }
  
  template <typename T, typename U, typename... Args>
  void CommandFactory<T, U, Args...>::listCommands() const
  {
    std::cout << "Commands enabled :" << std::endl;
    std::for_each(_commands.cbegin(),
		  _commands.cend(),
		  [](const std::pair
		     <std::string, CommandPtr<U, Args...>>& pair)
		  {
		    std::cout << pair.first << std::endl;
		  });
  }
}

#endif /* !COMMAND_FACTORY_HPP_ */
