#ifndef ERROR_OPENING_FILE_EXCEPTION_HH_
# define ERROR_OPENING_FILE_EXCEPTION_HH_

# include <string>
# include <stdexcept>

namespace Exception
{
  namespace Persistence
  {
    class ErrorOpeningFileException : public std::runtime_error
    {
    public :
      ErrorOpeningFileException(const std::string&);
      virtual ~ErrorOpeningFileException();
    };
  }
}

#endif /* !ERROR_OPENING_FILE_EXCEPTION_HH_ */
