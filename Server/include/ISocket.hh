#ifndef ISOCKET_HH_
# define ISOCKET_HH_

namespace Network
{ 
  class ISocket
  {
  public :
    virtual ~ISocket() { }
    virtual bool open(int, int, int) = 0;
    virtual bool close() = 0;
  };
}

#endif /* !ISOCKET_HH_ */
