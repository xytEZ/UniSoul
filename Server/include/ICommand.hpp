#ifndef ICOMMAND_HPP_
# define ICOMMAND_HPP_

# include <memory>

namespace Command
{ 
  template <typename T, typename... Args>
  class ICommand
  {
  public :
    virtual ~ICommand() = default;
    virtual T execute(Args&...) const = 0;
  };

  enum Command
    {
      NONE = 0,
      CONNECTION = 1,
      DECONNECTION = 2
    };
  
  template <typename T, typename... Args>
  using CommandPtr = std::shared_ptr<ICommand<T, Args...>>;
}

#endif /* !ICOMMAND_HPP_ */
