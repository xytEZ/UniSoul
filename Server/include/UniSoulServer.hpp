#ifndef UNI_SOUL_SERVER_HPP_
# define UNI_SOUL_SERVER_HPP_

# define VAR_NAME(x)	#x

# include <iostream>
# include <memory>
# include <typeinfo>
# include "IApp.hh"
# include "SocketManager.hpp"
# include "CommandExecutor.hpp"
# include "CommandFactory.hpp"
# include "TCPBoostSocketServer.hpp"
# include "PersistentDataFileInteractor.hh"
# include "UniSoulChannelSystem.hh"
# include "BoostServiceWrapper.hh"
# include "IComplexSystem.hh"

namespace App
{
  template <typename T, typename U = System::IComplexSystem, int N = 128>
  class UniSoulServer : public IApp
  {
  private :
    using LibraryServiceWrapper = std::unique_ptr<Wrapper::IWrapper<T>>;
    using SocketServer = std::unique_ptr<Network::ITCPSocketServer<std::shared_ptr<Network::ITCPSocket>>>;
    using SocketManager = Network::SocketManager<std::unique_ptr<Network::ITCPSocket>>;
    using ChannelSystem = std::unique_ptr<Communication::IChannelSystem>;
    using PersistentDataInteractor = std::unique_ptr<Persistence::IPersistentDataInteractor>;
    using CommandFactory = Command::CommandFactory<std::shared_ptr<U>>;
    using CommandExecutor = Command::CommandExecutor<std::shared_ptr<U>>;

  public :
    struct UniSoulSystem : public System::IComplexSystem
    {
      SocketServer		_socketServer;
      SocketManager		_socketManager;
      ChannelSystem		_channelSystem;
      PersistentDataInteractor	_persistentDataInteractor;
      CommandFactory		_commandFactory;
      CommandExecutor		_commandExecutor;

      UniSoulSystem(SocketServer&& socketServer,
		    ChannelSystem&& channelSystem,
		    PersistentDataInteractor&& persistentDataInteractor);
      virtual ~UniSoulSystem();
      virtual void toString() const;
    };

  private :
    using ComplexSystem = std::shared_ptr<System::IComplexSystem>;

  private :
    LibraryServiceWrapper	_libraryServiceWrapper;
    ComplexSystem		_complexSystem;
    std::string			_hostname;
    int				_port;
        
  public :
    UniSoulServer(const std::string&, int);
    virtual ~UniSoulServer();
    virtual bool init();
    virtual bool run();
    virtual bool close();
  };

  template <typename T, typename U, int N>
  UniSoulServer<T, U, N>::UniSoulSystem::UniSoulSystem(SocketServer&& socketServer,
						       ChannelSystem&& channelSystem,
						       PersistentDataInteractor&& persistentDataInteractor) :
    _socketServer(std::move(socketServer)),
    _channelSystem(std::move(channelSystem)),
    _persistentDataInteractor(std::move(persistentDataInteractor))
  {
  }

  template <typename T, typename U, int N>
  UniSoulServer<T, U, N>::UniSoulSystem::~UniSoulSystem() { }

  template <typename T, typename U, int N>
  void UniSoulServer<T, U, N>::UniSoulSystem::toString() const
  {
    std::cout << VAR_NAME(_socketServer) << " : " << typeid(_socketServer).name() << std::endl
	      << VAR_NAME(_socketManager) << " : " << typeid(_socketManager).name() << std::endl
	      << VAR_NAME(_channelSystem) << " : " << typeid(_channelSystem).name() << std::endl
	      << VAR_NAME(_persistentDataInteractor) << " : " << typeid(_persistentDataInteractor).name() << std::endl
	      << VAR_NAME(_commandFactory) << " : " << typeid(_commandFactory).name() << std::endl
	      << VAR_NAME(_commandExecutor) << " : " << typeid(_commandExecutor).name() << std::endl;
  }
  
  template <typename T, typename U, int N>
  UniSoulServer<T, U, N>::UniSoulServer(const std::string& hostname,
					int port) :
    _libraryServiceWrapper(std::make_unique<Wrapper::BoostServiceWrapper>()),
    _complexSystem(std::make_shared<UniSoulSystem>(
						   std::make_unique<Network::TCPBoostSocketServer<N, std::shared_ptr<Network::ITCPSocket>>>(_libraryServiceWrapper->getContent(),
																	    _complexSystem,
																	    hostname,
																	    port),
						   std::make_unique<Communication::UniSoulChannelSystem>(),
						   std::make_unique<Persistence::PersistentDataFileInteractor>()
						   )),
    _hostname(hostname),
    _port(port)
  {
  }

  template <typename T, typename U, int N>
  UniSoulServer<T, U, N>::~UniSoulServer() { }

  template <typename T, typename U, int N>
  bool UniSoulServer<T, U, N>::init()
  {
    std::static_pointer_cast<UniSoulSystem>(_complexSystem)->_socketServer->open(0, 0, 0);
    return true;
  }

  template <typename T, typename U, int N>
  bool UniSoulServer<T, U, N>::run()
  {
    bool	libraryServiceIsExisting = _libraryServiceWrapper;

    do
      {
	std::static_pointer_cast<UniSoulSystem>(_complexSystem)->_socketServer->accept(nullptr, nullptr);
      } while (!libraryServiceIsExisting);
    if (libraryServiceIsExisting)
      _libraryServiceWrapper->getContent().run();
    return true;
  }

  template <typename T, typename U, int N>
  bool UniSoulServer<T, U, N>::close()
  {
    std::static_pointer_cast<UniSoulSystem>(_complexSystem)->_socketServer->close();
    return true;
  }
}

#endif /* !UNI_SOUL_SERVER_HPP_ */
