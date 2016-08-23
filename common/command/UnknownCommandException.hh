#ifndef UNKNOWN_COMMAND_EXCEPTION_HH_
# define UNKNOWN_COMMAND_EXCEPTION_HH_

# include <string>
# include <stdexcept>

namespace Exception
{
  namespace Command
  {
    class UnknownCommand : public std::runtime_error
    {
    public :
      UnknownCommand(const std::string&);
    };
  }
}

#endif /* !UNKNOWN_COMMAND_EXCEPTION_HH_ */
