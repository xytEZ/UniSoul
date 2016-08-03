#ifndef EPOLL_HH_
# define EPOLL_HH_

# include <sys/epoll.h>

# include "IMultiplexer.hh"

namespace Network
{
  class Epoll : public IMultiplexer
  {
  private :
    using EpollEvent = struct epoll_event;
    
  private :
    int						_epollFd;
    int						_resNbr;
    int						_nbrMax;
    int						_timeOut;
    EpollEvent					*_eventsRes;
    EpollEvent					_events[2];
    IMultiplexer::ioCallback			_callbacks[2];
    std::list<IMultiplexer::SocketCallback>	_polled;

  public :
    Epoll(const IMultiplexer::ioCallback&,
	  const IMultiplexer::ioCallback&,
	  int,
	  int);
    
    virtual ~Epoll();
    virtual void addSocket(const SocketPtr&, IMultiplexer::Flag);
    virtual void addSocket(const SocketPtr&,
			   const IMultiplexer::ioCallback&,
			   IMultiplexer::Flag);
    virtual void addSockets(const std::list<IMultiplexer::SocketPtr>&,
			    IMultiplexer::Flag);
    virtual void addSockets(const std::list<IMultiplexer::SocketPtr>&,
			    const IMultiplexer::ioCallback&,
			    IMultiplexer::Flag);
    virtual int process();
    virtual void execute();
    virtual void clear();

  private :
    void closeSocket(const IMultiplexer::SocketCallback&);
  };
}

#endif /* !EPOLL_HH_ */
