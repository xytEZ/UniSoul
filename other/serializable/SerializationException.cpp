#include "SerializationException.hh"

namespace Exception
{
  SerializationException::SerializationException(const std::string& what) :
    _what(what)
  {
  }

  const char *SerializationException::what() const throw()
  {
    return _what.c_str();
  }
}
