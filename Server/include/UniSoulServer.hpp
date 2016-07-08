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
  template <typename T, typename U = System::IComplexSystem, int N = 128, int N2 = 60>
  class UniSoulServer : public IApp
  {
  private :
    using LibraryServiceWrapper = std::unique_ptr<Wrapper::IWrapper<T>>;
    using SocketServer = std::unique_ptr<Network::ITCPSocketServer<std::shared_ptr<Network::ITCPSocket>>>;
    using SocketManager = Network::SocketManager<std::unique_ptr<Network::ITCPSocket>>;
    using PersistentDataInteractor = std::unique_ptr<Persistence::IPersistentDataInteractor>;
    using ChannelSystem = std::unique_ptr<Communication::IChannelSystem>;
    using CommandFactory = Command::CommandFactory<std::shared_ptr<U>>;
    using CommandExecutor = Command::CommandExecutor<std::shared_ptr<U>>;

  public :
    struct UniSoulSystem : public System::IComplexSystem
    {
      SocketServer		_socketServer;
      SocketManager		_socketManager;
      PersistentDataInteractor	_persistentDataInteractor;
      ChannelSystem		_channelSystem;
      CommandFactory		_commandFactory;
      CommandExecutor		_commandExecutor;

      UniSoulSystem(SocketServer&&, PersistentDataInteractor&&, ChannelSystem&&);
      virtual ~UniSoulSystem();
      virtual void toString() const;
    };

  private :
    using ComplexSystem = std::unique_ptr<System::IComplexSystem>;

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

  template <typename T, typename U, int N, int N2>
  UniSoulServer<T, U, N, N2>::UniSoulSystem::UniSoulSystem(SocketServer&& socketServer,
						       PersistentDataInteractor&& persistentDataInteractor,
						       ChannelSystem&& channelSystem) :
    _socketServer(std::move(socketServer)),
    _persistentDataInteractor(std::move(persistentDataInteractor)),
    _channelSystem(std::move(channelSystem))
  {
  }

  template <typename T, typename U, int N, int N2>
  UniSoulServer<T, U, N, N2>::UniSoulSystem::~UniSoulSystem() { }

  template <typename T, typename U, int N, int N2>
  void UniSoulServer<T, U, N, N2>::UniSoulSystem::toString() const
  {
    std::cout << VAR_NAME(_socketServer) << " : " << typeid(_socketServer).name() << std::endl
	      << VAR_NAME(_socketManager) << " : " << typeid(_socketManager).name() << std::endl
	      << VAR_NAME(_channelSystem) << " : " << typeid(_channelSystem).name() << std::endl
	      << VAR_NAME(_persistentDataInteractor) << " : " << typeid(_persistentDataInteractor).name() << std::endl
	      << VAR_NAME(_commandFactory) << " : " << typeid(_commandFactory).name() << std::endl
	      << VAR_NAME(_commandExecutor) << " : " << typeid(_commandExecutor).name() << std::endl;
  }
  
  template <typename T, typename U, int N, int N2>
  UniSoulServer<T, U, N, N2>::UniSoulServer(const std::string& hostname,
					int port) :
    _libraryServiceWrapper(std::make_unique<Wrapper::BoostServiceWrapper>()),
    _complexSystem(std::make_unique<UniSoulSystem>(
						   std::make_unique<Network::TCPBoostSocketServer<N, N2, std::shared_ptr<Network::ITCPSocket>>>(_libraryServiceWrapper->getContent(),
																		_complexSystem,
																		hostname,
																		port),
						   std::make_unique<Persistence::PersistentDataFileInteractor>(),
						   std::make_unique<Communication::UniSoulChannelSystem>()
						   )),
    _hostname(hostname),
    _port(port)
  {
  }

  template <typename T, typename U, int N, int N2>
  UniSoulServer<T, U, N, N2>::~UniSoulServer() { }

  template <typename T, typename U, int N, int N2>
  bool UniSoulServer<T, U, N, N2>::init()
  {
    static_cast<UniSoulSystem *>(_complexSystem.get())->_socketServer->open(0, 0, 0);
    return true;
  }

  template <typename T, typename U, int N, int N2>
  bool UniSoulServer<T, U, N, N2>::run()
  {
    static_cast<UniSoulSystem *>(_complexSystem.get())->_socketServer->accept(nullptr, nullptr);
    _libraryServiceWrapper->getContent().run();
    return true;
  }

  template <typename T, typename U, int N, int N2>
  bool UniSoulServer<T, U, N, N2>::close()
  {
    static_cast<UniSoulSystem *>(_complexSystem.get())->_socketServer->close();
    static_cast<UniSoulSystem *>(_complexSystem.get())
      ->_socketManager.apply([](const std::unique_ptr<Network::ITCPSocket>& socket)
			     {
			       dynamic_cast<Network::TCPBoostSocket<N, N2> *>(socket.get())->close();
			     });
    return true;
  }
}

#endif /* !UNI_SOUL_SERVER_HPP_ */
