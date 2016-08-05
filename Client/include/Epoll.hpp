#ifndef EPOLL_HPP_
# define EPOLL_HPP_

# include <array>
# include <system_error>
# include <algorithm>

# include <cerrno>
# include <unistd.h>
# include <sys/epoll.h>

# include "IMultiplexer.hh"

namespace Network
{
  template <std::size_t N>
  class Epoll : public IMultiplexer
  { 
  private :
    int						_epollFd;
    int						_resNbr;
    int						_nbrMax;
    int						_timeOut;
    std::array<struct epoll_event, N>		_eventsRes;
    struct epoll_event				_events[2];
    IMultiplexer::ioCallback			_callbacks[2];
    std::list<IMultiplexer::SocketCallback>	_polled;
    
  public :
    Epoll(const IMultiplexer::ioCallback&,
	  const IMultiplexer::ioCallback&,
	  int);
    
    virtual ~Epoll();
    virtual void addSocket(const IMultiplexer::SocketPtr&, IMultiplexer::Flag);
    
    virtual void addSocket(const IMultiplexer::SocketPtr&,
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

  template <std::size_t N>
  Epoll<N>::Epoll(const IMultiplexer::ioCallback& read,
		  const IMultiplexer::ioCallback& write,
		  int timeOut) :
    _timeOut(timeOut)
  {
    if ((_epollFd = ::epoll_create(N)) == -1)
      throw std::system_error(errno, std::system_category());
    _events[IMultiplexer::READ].events = EPOLLIN | EPOLLPRI;
    _callbacks[IMultiplexer::READ] = std::move(read);
    _events[IMultiplexer::WRITE].events = EPOLLOUT;
    _callbacks[IMultiplexer::WRITE] = std::move(write);
  }

  template <std::size_t N>
  Epoll<N>::~Epoll()
  {
    ::close(_epollFd);
  }
  
  template <std::size_t N>
  void Epoll<N>::addSocket(const IMultiplexer::SocketPtr& socketPtr,
			   IMultiplexer::Flag flag)
  {
    IMultiplexer::SocketCallback        socketCallback;
    
    socketCallback.socketPtr = socketPtr;
    socketCallback.callback = _callbacks[flag];
    _events[flag].data.ptr = &socketCallback.socketPtr;

    if (::epoll_ctl(_epollFd,
		    EPOLL_CTL_ADD,
		    socketCallback.socketPtr->getDescriptor(),
		    &_events[flag]) == -1)
      throw std::system_error(errno, std::system_category());
    _polled.push_back(std::move(socketCallback));
  }

  template <std::size_t N>
  void Epoll<N>::addSocket(const IMultiplexer::SocketPtr& socketPtr,
			   const IMultiplexer::ioCallback& callback,
			   IMultiplexer::Flag flag)
  {
    IMultiplexer::SocketCallback        socketCallback;

    socketCallback.socketPtr = socketPtr;
    socketCallback.callback = callback;
    _events[flag].data.ptr = &socketCallback.socketPtr;
    if (::epoll_ctl(_epollFd,
		    EPOLL_CTL_ADD,
		    socketCallback.socketPtr->getDescriptor(),
		    &_events[flag]) == -1)
      throw std::system_error(errno, std::system_category());
    _polled.push_back(std::move(socketCallback));
  }
  
  template <std::size_t N>
  void Epoll<N>::addSockets(const std::list
			    <IMultiplexer::SocketPtr>& socketsPtr,
			    IMultiplexer::Flag flag)
  {
    std::for_each(socketsPtr.cbegin(),
		  socketsPtr.cend(),
		  [this, flag](const IMultiplexer::SocketPtr& socketPtr)
		  -> void
		  {
		    addSocket(socketPtr, flag);
		  });
  }
  
  template <std::size_t N>
  void Epoll<N>::addSockets(const std::list
			    <IMultiplexer::SocketPtr>& socketsPtr,
			    const IMultiplexer::ioCallback& callback,
			    IMultiplexer::Flag flag)
  {
    std::for_each(socketsPtr.cbegin(),
		  socketsPtr.cend(),
		  [this, &callback, flag]
		  (const IMultiplexer::SocketPtr& socketPtr) -> void
		  {
		    addSocket(socketPtr, callback, flag);
		  });
  }
  
  template <std::size_t N>
  int Epoll<N>::process()
  {
    if ((_resNbr = ::epoll_wait(_epollFd,
				_eventsRes.data(),
				N,
				_timeOut)) == -1)
      throw std::system_error(errno, std::system_category());
    return _resNbr;
  }
  
  template <std::size_t N>
  void Epoll<N>::execute()
  {
    IMultiplexer::SocketCallback        socketCallback;
    
    for (int n = 0; n < _resNbr; ++n)
      {
	socketCallback =
	  *reinterpret_cast<IMultiplexer::SocketCallback *>(_eventsRes[n]
							    .data
							    .ptr);
      }
  }

  template <std::size_t N>
  void Epoll<N>::clear()
  {
    std::for_each(_polled.cbegin(),
		  _polled.cend(),
		  [this](const IMultiplexer::SocketCallback& socketCallback)
		  -> void
		  {
		    if (::epoll_ctl(_epollFd,
				    EPOLL_CTL_DEL,
				    socketCallback.socketPtr->getDescriptor(),
				    0) == -1)
		      throw std::system_error(errno, std::system_category());
		  });
    _polled.clear();
  }

  template <std::size_t N>
  void Epoll<N>::closeSocket(const IMultiplexer::SocketCallback&
			     socketCallback)
  {
    if (::epoll_ctl(_epollFd,
		    EPOLL_CTL_DEL,
		    socketCallback.socketPtr->getDescriptor(),
		    0) == -1)
      throw std::system_error(errno, std::system_category());
    std::remove_if(_polled.begin(),
		   _polled.end(),
		   [&socketCallback]
		   (const IMultiplexer::SocketCallback& socketCallback2)
		   -> bool
		   {
		     return socketCallback.socketPtr
		       == socketCallback2.socketPtr;
		   });
    socketCallback.socketPtr->close();
  }
}

#endif /* !EPOLL_HPP_ */
