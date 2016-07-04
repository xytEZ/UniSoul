#ifndef UNI_SOUL_SERVER_HPP_
# define UNI_SOUL_SERVER_HPP_

# include <memory>
# include "IApp.hh"
# include "SocketManager.hpp"
# include "CommandExecutor.hpp"
# include "CommandFactory.hpp"
# include "TCPBoostSocketServer.hpp"
# include "PersistentDataFileInteractor.hh"
# include "UniSoulChannelSystem.hh"
# include "BoostServiceWrapper.hh"

namespace App
{
  template <typename T, int N = 128>
  class UniSoulServer : public IApp
  {
  private :
    using LibraryServiceWrapper = std::unique_ptr<Wrapper::ILibraryServiceWrapper<T>>;
    using SocketServer = std::unique_ptr<Network::ITCPSocketServer<std::shared_ptr<Network::ITCPSocket>>>;
    using SocketManager = Network::SocketManager<std::unique_ptr<Network::ITCPSocket>>;
    using ChannelSystem = std::unique_ptr<Communication::IChannelSystem>;
    using PersistentDataInteractor = std::unique_ptr<Persistence::IPersistentDataInteractor>;
    using CommandFactory = Command::CommandFactory<IApp>;
    using CommandExecutor = Command::CommandExecutor<IApp>;

  private :
    LibraryServiceWrapper	_libraryServiceWrapper;
    SocketServer		_socketServer;
    SocketManager		_socketManager;
    ChannelSystem		_channelSystem;
    PersistentDataInteractor	_persistentDataInteractor;
    CommandFactory		_commandFactory;
    CommandExecutor		_commandExecutor;
    std::string			_hostname;
    int				_port;
        
  public :
    UniSoulServer(const std::string&, int);
    virtual ~UniSoulServer();
    virtual bool init();
    virtual bool run();
    virtual bool close();
  };

  template <typename T, int N>
  UniSoulServer<T, N>::UniSoulServer(const std::string& hostname, int port) :
    _libraryServiceWrapper(std::make_unique<Wrapper::BoostServiceWrapper>()),
    _socketServer(std::make_unique<Network::TCPBoostSocketServer<N, std::shared_ptr<Network::ITCPSocket>>>(_libraryServiceWrapper->getContent(),
													   hostname,
													   port)),
    _socketManager(),
    _channelSystem(std::make_unique<Communication::UniSoulChannelSystem>()),
    _persistentDataInteractor(std::make_unique<Persistence::PersistentDataFileInteractor>()),
    _commandFactory(),
    _commandExecutor(),
    _hostname(hostname),
    _port(port)
  {
  }

  template <typename T, int N>
  UniSoulServer<T, N>::~UniSoulServer() { }

  template <typename T, int N>
  bool UniSoulServer<T, N>::init()
  {
    _socketServer->open(0, 0, 0);
    return true;
  }

  template <typename T, int N>
  bool UniSoulServer<T, N>::run()
  {
    _libraryServiceWrapper->getContent().run();
    return true;
  }

  template <typename T, int N>
  bool UniSoulServer<T, N>::close()
  {
    _socketServer->close();
    return true;
  }
}

#endif /* !UNI_SOUL_SERVER_HPP_ */
