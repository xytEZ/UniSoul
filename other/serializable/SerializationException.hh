#ifndef SERIALIZATION_EXCEPTION_HH_
# define SERIALIZATION_EXCEPTION_HH_

# include <string>
# include <exception>

namespace Exception
{
  class SerializationException : public std::exception
  {
  private :
    std::string	_what;

  public :
    SerializationException(const std::string&);
    ~SerializationException() = default;
    virtual const char *what() const throw();
  };
}

#endif /* !SERIALIZATION_EXCEPTION_HH_ */
