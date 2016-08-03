#include <system_error>
#include <unistd.h>
#include <cerrno>
#include <algorithm>

#include "Epoll.hh"

namespace Network
{
  Epoll::Epoll(const IMultiplexer::ioCallback& read,
	       const IMultiplexer::ioCallback& write,
	       int nbrMax,
	       int timeOut) :
    _nbrMax(nbrMax),
    _timeOut(timeOut)
  {
    if ((_epollFd = ::epoll_create(nbrMax)) == -1)
      throw std::system_error(errno, std::system_category());
    _eventsRes = new struct epoll_event[nbrMax];
    _events[IMultiplexer::READ].events = EPOLLIN | EPOLLPRI;
    _callbacks[IMultiplexer::READ] = std::move(read);
    _events[IMultiplexer::WRITE].events = EPOLLOUT;
    _callbacks[IMultiplexer::WRITE] = std::move(write);
  }

  Epoll::~Epoll()
  {
    delete[] _eventsRes;
    ::close(_epollFd);
    _polled.clear();
  }

  void Epoll::addSocket(const SocketPtr& socketPtr, IMultiplexer::Flag flag)
  {
    IMultiplexer::SocketCallback	socketCallback;

    socketCallback.socketPtr = std::move(socketPtr);
    socketCallback.callback = _callbacks[flag];
    _events[flag].data.ptr = &socketCallback.socketPtr;
    
    if (::epoll_ctl(_epollFd,
		    EPOLL_CTL_ADD,
		    socketCallback.socketPtr->getDescriptor(),
		    &_events[flag]) == -1)
      throw std::system_error(errno, std::system_category());
    _polled.push_back(std::move(socketCallback));
  }
  
  void Epoll::addSocket(const SocketPtr& socketPtr,
			const IMultiplexer::ioCallback& callback,
			IMultiplexer::Flag flag)
  {
    IMultiplexer::SocketCallback	socketCallback;

    socketCallback.socketPtr = std::move(socketPtr);
    socketCallback.callback = std::move(callback);
    _events[flag].data.ptr = &socketCallback.socketPtr;

    if (::epoll_ctl(_epollFd,
		    EPOLL_CTL_ADD,
		    socketCallback.socketPtr->getDescriptor(),
		    &_events[flag]) == -1)
      throw std::system_error(errno, std::system_category());
    _polled.push_back(std::move(socketCallback));
  }

  void Epoll::addSockets(const std::list<IMultiplexer::SocketPtr>& socketsPtr,
			 IMultiplexer::Flag flag)
  {
    std::for_each(socketsPtr.cbegin(),
		  socketsPtr.cend(),
		  [this, flag](const SocketPtr& socketPtr) -> void
		  {
		    this->addSocket(socketPtr, flag);
		  });
  }

  void Epoll::addSockets(const std::list<IMultiplexer::SocketPtr>& socketsPtr,
			 const IMultiplexer::ioCallback& callback,
			 IMultiplexer::Flag flag)
  {
    std::for_each(socketsPtr.cbegin(),
		  socketsPtr.cend(),
		  [this, &callback, flag](const SocketPtr& socketPtr) -> void
		  {
		    this->addSocket(socketPtr, callback, flag);
		  });
  }
  
  int Epoll::process()
  {
    if ((_resNbr = ::epoll_wait(_epollFd,
				_eventsRes,
				_nbrMax,
				_timeOut)) == -1)
      throw std::system_error(errno, std::system_category());
    return _resNbr;
  }
  
  void Epoll::execute()
  {
    IMultiplexer::SocketCallback	socketCallback;
    
    for (int n = 0; n < _resNbr; ++n)
      {
	socketCallback =
	  *reinterpret_cast<IMultiplexer::SocketCallback *>(_eventsRes[n]
							    .data
							    .ptr);
      }
  }
  
  void Epoll::clear()
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

  void Epoll::closeSocket(const IMultiplexer::SocketCallback& socketCallback)
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
