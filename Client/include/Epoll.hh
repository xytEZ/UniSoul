#ifndef EPOLL_HH_
# define EPOLL_HH_

namespace Network
{
  class Epoll : public IMultiplexer
  {
  private :
    std::list<IMultiplexer::SocketCallBack>	_polled;
    
  public :
    virtual ~Epoll();
    virtual void addSocket(const SocketPtr&, IMultiplexer::Flag);
    virtual void addSocket(const SocketPtr&,
			   const IMultiplexer::ioCallback&,
			   IMultiplexer::Flag);
    virtual void addSockets(const std::list<IMultiplexer::SocketPtr>&,
			    IMultiplexer::Flag);
    virtual void addSockets(const std::list<IMultiplexer::SocketPtr>&,
			    const IMultiplexer::ioCallBack&,
			    IMultiplexer::Flag);
    virtual void clear();
    virtual void execute();
  };
}

#endif /* !EPOLL_HH_ */
