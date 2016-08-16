#ifndef ERROR_WITH_CONNECTION_EXCEPTION_HH_
# define ERROR_WITH_CONNECTION_EXCEPTION_HH_

# include <string>
# include <stdexcept>

namespace Exception
{
  namespace Network
  {
    class ErrorWithConnection : public std::runtime_error
    {
    public :
      ErrorWithConnection(const std::string&);
    };
  }
}

#endif /* !ERROR_WITH_CONNECTION_EXCEPTION_HH_ */
