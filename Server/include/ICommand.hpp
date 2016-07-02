#ifndef ICOMMAND_HPP_
# define ICOMMAND_HPP_

namespace Command
{
  template <typename T>
  class ICommand
  {
  public :
    virtual ~ICommand() { }
    virtual bool execute(T&) const = 0;
  };
}

#endif /* !ICOMMAND_HPP_ */
