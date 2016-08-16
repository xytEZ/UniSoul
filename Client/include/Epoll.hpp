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
  template <int TIMEOUT, std::size_t N>
  class Epoll : public IMultiplexer
  {
  private :
    using SocketPtr = std::shared_ptr<Network::ISocket<int>>;
    
  private :
    int						_epollFd;
    int						_resNbr;
    int						_nbrMax;
    std::array<struct epoll_event, N>		_eventsRes;
    struct epoll_event				_events[2];
    IMultiplexer::ioCallback			_callbacks[2];
    std::list<IMultiplexer::SocketCallbackPtr>	_polled;
    
  public :
    Epoll(const IMultiplexer::ioCallback&, const IMultiplexer::ioCallback&);
    virtual ~Epoll();
    virtual void addSocket(const SocketPtr&, IMultiplexer::Flag);
    
    virtual void addSocket(const SocketPtr&,
			   const IMultiplexer::ioCallback&,
			   IMultiplexer::Flag);
    
    virtual void addSockets(const std::list<SocketPtr>&, IMultiplexer::Flag);
    
    virtual void addSockets(const std::list<SocketPtr>&,
			    const IMultiplexer::ioCallback&,
			    IMultiplexer::Flag);
    
    virtual int process();
    virtual void execute();
    virtual void clear();

  private :
    void closeSocket(const IMultiplexer::SocketCallback *);
  };

  template <int TIMEOUT, std::size_t N>
  Epoll<TIMEOUT, N>::Epoll(const IMultiplexer::ioCallback& read,
			   const IMultiplexer::ioCallback& write)
  {
    if ((_epollFd = ::epoll_create(N)) == -1)
      throw std::system_error(errno, std::system_category());
    _events[IMultiplexer::READ].events = EPOLLIN | EPOLLPRI;
    _callbacks[IMultiplexer::READ] = std::move(read);
    _events[IMultiplexer::WRITE].events = EPOLLOUT;
    _callbacks[IMultiplexer::WRITE] = std::move(write);
  }

  template <int TIMEOUT, std::size_t N>
  Epoll<TIMEOUT, N>::~Epoll()
  {
    if (::close(_epollFd) == -1)
      throw std::system_error(errno, std::system_category());
  }
  
  template <int TIMEOUT, std::size_t N>
  void Epoll<TIMEOUT, N>::addSocket(const SocketPtr& socketPtr,
				    IMultiplexer::Flag flag)
  {
    IMultiplexer::SocketCallbackPtr	socketCallbackPtr =
      std::make_shared<IMultiplexer::SocketCallback>();
    
    socketCallbackPtr->socketPtr = socketPtr;
    socketCallbackPtr->callback = _callbacks[flag];
    _events[flag].data.ptr = socketCallbackPtr.get();
    if (::epoll_ctl(_epollFd,
		    EPOLL_CTL_ADD,
		    socketCallbackPtr->socketPtr->getDescriptor(),
		    &_events[flag]) == -1)
      throw std::system_error(errno, std::system_category());
    _polled.push_back(socketCallbackPtr);
  }
  
  template <int TIMEOUT, std::size_t N>
  void Epoll<TIMEOUT, N>::addSocket(const SocketPtr& socketPtr,
				    const IMultiplexer::ioCallback& callback,
				    IMultiplexer::Flag flag)
  {
    IMultiplexer::SocketCallbackPtr	socketCallbackPtr =
      std::make_shared<IMultiplexer::SocketCallback>();

    socketCallbackPtr->socketPtr = socketPtr;
    socketCallbackPtr->callback = callback;
    _events[flag].data.ptr = socketCallbackPtr.get();
    if (::epoll_ctl(_epollFd,
		    EPOLL_CTL_ADD,
		    socketCallbackPtr->socketPtr->getDescriptor(),
		    &_events[flag]) == -1)
      throw std::system_error(errno, std::system_category());
    _polled.push_back(socketCallbackPtr);
  }
  
  template <int TIMEOUT, std::size_t N>
  void Epoll<TIMEOUT, N>::addSockets(const std::list<SocketPtr>& socketsPtr,
			    IMultiplexer::Flag flag)
  {
    std::for_each(socketsPtr.cbegin(),
		  socketsPtr.cend(),
		  [this, flag](const SocketPtr& socketPtr) -> void
		  {
		    addSocket(socketPtr, flag);
		  });
  }
  
  template <int TIMEOUT, std::size_t N>
  void Epoll<TIMEOUT, N>::addSockets(const std::list<SocketPtr>& socketsPtr,
				     const IMultiplexer::ioCallback& callback,
				     IMultiplexer::Flag flag)
  {
    std::for_each(socketsPtr.cbegin(),
		  socketsPtr.cend(),
		  [this, &callback, flag](const SocketPtr& socketPtr) -> void
		  {
		    addSocket(socketPtr, callback, flag);
		  });
  }
  
  template <int TIMEOUT, std::size_t N>
  int Epoll<TIMEOUT, N>::process()
  {
    if ((_resNbr = ::epoll_wait(_epollFd,
				_eventsRes.data(),
				N,
				TIMEOUT)) == -1)
      throw std::system_error(errno, std::system_category());
    return _resNbr;
  }
  
  template <int TIMEOUT, std::size_t N>
  void Epoll<TIMEOUT, N>::execute()
  {
    IMultiplexer::SocketCallback	*socketCallback;

    for (int n = 0; n < _resNbr; ++n)
      {
	socketCallback = 
	  reinterpret_cast<IMultiplexer::SocketCallback *>
	  (_eventsRes[n].data.ptr);
	if (!(_eventsRes[n].events & EPOLLERR)
	    && !(_eventsRes[n].events & EPOLLHUP))
	  socketCallback->callback(socketCallback->socketPtr);
	else if (_eventsRes[n].events & EPOLLHUP)
	  closeSocket(socketCallback);
      }
  }
  
  template <int TIMEOUT, std::size_t N>
  void Epoll<TIMEOUT, N>::clear()
  {
    std::for_each(_polled.cbegin(),
		  _polled.cend(),
		  [this]
		  (const IMultiplexer::SocketCallbackPtr& socketCallbackPtr)
		  -> void
		  {
		    if (::epoll_ctl(_epollFd,
				    EPOLL_CTL_DEL,
				    socketCallbackPtr->socketPtr
				    ->getDescriptor(),
				    0) == -1)
		      throw std::system_error(errno, std::system_category());
		  });
    _polled.clear();
  }

  template <int TIMEOUT, std::size_t N>
  void
  Epoll<TIMEOUT, N>
  ::closeSocket(const IMultiplexer::SocketCallback *socketCallback)
  {
    if (::epoll_ctl(_epollFd,
		    EPOLL_CTL_DEL,
		    socketCallback->socketPtr->getDescriptor(),
		    0) == -1)
      throw std::system_error(errno, std::system_category());
    std::remove_if(_polled.begin(),
		   _polled.end(),
		   [&socketCallback]
		   (const IMultiplexer::SocketCallbackPtr& socketCallbackPtr)
		   -> bool
		   {
		     return socketCallbackPtr->socketPtr
		       == socketCallback->socketPtr;
		   });
    socketCallback->socketPtr->close();
  }
}

#endif /* !EPOLL_HPP_ */
