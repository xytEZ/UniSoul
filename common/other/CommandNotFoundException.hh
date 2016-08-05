#ifndef COMMAND_NOT_FOUND_EXCEPTION_HH_
# define COMMAND_NOT_FOUND_EXCEPTION_HH_

# include <string>
# include <stdexcept>

namespace Exception
{
  namespace Command
  {
    class CommandNotFoundException : public std::runtime_error
    {
    public :
      CommandNotFoundException(const std::string&);
    };
  }
}

#endif /* !COMMAND_NOT_FOUND_EXCEPTION_HH_ */
