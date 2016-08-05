#ifndef ISOCKET_HPP_
# define ISOCKET_HPP_

namespace Network
{
  template <typename T>
  class ISocket
  {
  public :
    virtual ~ISocket() = default;
    virtual bool open() = 0;
    virtual bool close() = 0;
    virtual const T& getDescriptor() const = 0;
  };
}

#endif /* !ISOCKET_HPP_ */
