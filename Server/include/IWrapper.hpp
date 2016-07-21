#ifndef IWRAPPER_HPP_
# define IWRAPPER_HPP_

namespace Wrapper
{
  template <typename T>
  class IWrapper
  {
  public :
    virtual ~IWrapper() = default;
    virtual T& getContent() = 0;
  };
}

#endif /* !IWRAPPER_HPP_ */
