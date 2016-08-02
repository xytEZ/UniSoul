#ifndef IMULTIPLEXER_HH_
# define IMULTIPLEXER_HH_

# include <memory>
# include <functional>
# include <list>
# include "ISocket.hh"

namespace Network
{ 
  class IMultiplexer
  {
  protected :
    using SocketPtr = std::unique_ptr<Network::ISocket>;

  public :
    using ioCallback = std::function<void(const SocketPtr&)>;
    
  public :
    struct SocketCallback
    {
      SocketPtr		socketPtr;
      ioCallback	callback;
    };
    
    enum Flag
      {
	READ,
	WRITE
      };

  public :
    virtual ~IMultiplexer() = default;
    virtual void addSocket(const SocketPtr&, Flag = Flag::READ) = 0;
    virtual void addSocket(const SocketPtr&,
			   const ioCallback&,
			   Flag = Flag::READ) = 0;
    virtual void addSockets(const std::list<SocketPtr>&,
			    Flag = Flag::READ) = 0;
    virtual void addSockets(const std::list<SocketPtr>&,
			    const ioCallback&,
			    Flag = Flag::READ) = 0;
    virtual void clear() = 0;
    virtual void execute() = 0;
  };
}

#endif /* !IMULTIPLEXER_HH_ */
