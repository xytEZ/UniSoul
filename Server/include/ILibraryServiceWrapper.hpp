#ifndef ILIBRARY_SERVICE_WRAPPER_HPP_
# define ILIBRARY_SERVICE_WRAPPER_HPP_

namespace Wrapper
{
  template <typename T>
  class ILibraryServiceWrapper
  {
  public :
    virtual ~ILibraryServiceWrapper() { };
    virtual T& getContent() = 0;
  };
}

#endif /* !ILIBRARY_SERVICE_WRAPPER_HPP__ */
