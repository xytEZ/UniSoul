#ifndef UNI_SOUL_SERVER_HH_
# define UNI_SOUL_SERVER_HH_

# include <memory>
# include "IApp.hh"
# include "SocketManager.hpp"
# include "CommandExecutor.hpp"
# include "CommandFactory.hpp"

namespace Network
{
  class ITCPSocket;

  template <typename T>
  class ITCPSocketServer;
}

namespace Persistence
{
  class IPersistentDataInteractor;
}

namespace Communication
{
  class IChannelSystem;
}

namespace App
{
  class UniSoulServer : public IApp
  {
  private :
    using SocketServer = std::unique_ptr<Network::ITCPSocketServer<std::unique_ptr<Network::ITCPSocket>>>;
    using PersistentDataInteractor = std::unique_ptr<Persistence::IPersistentDataInteractor>;
    using ChannelSystem = std::unique_ptr<Communication::IChannelSystem>;
    using SocketManager = Network::SocketManager<std::unique_ptr<Network::ITCPSocket>>;
    using CommandFactory = Command::CommandFactory<IApp>;
    using CommandExecutor = Command::CommandExecutor<IApp>;
    
  private :
    static constexpr const char	*HOSTNAME = "aries.kent.ac.uk";
    static constexpr const int	PORT = 4242;

  private :
    SocketServer		_socketServer;
    PersistentDataInteractor	_persistentDataInteractor;
    ChannelSystem		_channelSystem;
    SocketManager		_socketManager;
    CommandFactory		_commandFactory;
    CommandExecutor		_commandExecutor;
        
  public :
    UniSoulServer();
    virtual ~UniSoulServer();
    virtual bool init();
    virtual bool run();
    virtual bool close();
  };
}

#endif /* !UNI_SOUL_SERVER_HH_ */
