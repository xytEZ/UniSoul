#ifndef IMULTIPLEXER_HH_
# define IMULTIPLEXER_HH_

# include <memory>
# include <functional>
# include <list>

# include "ISocket.hpp"

namespace Network
{
  class IMultiplexer
  {
  private :
    using SocketPtr = std::shared_ptr<Network::ISocket<int>>;
    using SocketsPtr = std::list<SocketPtr>;
    
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
    using SocketCallbackPtr = std::shared_ptr<SocketCallback>;
    using SocketCallbacksPtr = std::list<SocketCallbackPtr>;

  public :
    virtual ~IMultiplexer() = default;
    virtual void addSocket(const SocketPtr&, Flag = Flag::READ) = 0;
    
    virtual void addSocket(const SocketPtr&,
			   const ioCallback&,
			   Flag = Flag::READ) = 0;
    
    virtual void addSockets(const SocketsPtr&, Flag = Flag::READ) = 0;
    
    virtual void addSockets(const SocketsPtr&,
			    const ioCallback&,
			    Flag = Flag::READ) = 0;
    
    virtual int process() = 0;
    virtual void execute() = 0;
    virtual void clear() = 0;
    virtual const SocketCallbacksPtr& getSocketCallbacksPtr() const = 0;
  };
}

#endif /* !IMULTIPLEXER_HH_ */
