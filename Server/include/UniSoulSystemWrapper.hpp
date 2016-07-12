#ifndef UNI_SOUL_SYSTEM_WRAPPER_HPP_
# define UNI_SOUL_SYSTEM_WRAPPER_HPP_

# include "ISystemWrapper.hh"
# include "PersistentDataFileInteractor.hh"
# include "UniSoulChannelSystem.hh"
# include "SocketManager.hpp"
# include "TCPBoostSocketServer.hpp"
# include "CommandFactory.hpp"
# include "CommandExecutor.hpp"

namespace Wrapper
{
  namespace System
  {
    template <typename T, int N, int N2>
    class UniSoulSystemWrapper : public ISystemWrapper
    { 
    public :
      using SocketServer = std::shared_ptr
	<Network::ITCPSocketServer
	 <std::shared_ptr<Network::ITCPSocket>>>;
      using SocketManager = Network::SocketManager
	<std::shared_ptr<Network::ITCPSocket>>;
      using PersistentDataInteractor = std::shared_ptr
	<Persistence::IPersistentDataInteractor>;
      using ChannelSystem = std::shared_ptr
	<Communication::IChannelSystem>;
      using CommandFactory = Command
	::CommandFactory<UniSoulSystemWrapper<T, N, N2>>;
      using CommandExecutor = Command
	::CommandExecutor<UniSoulSystemWrapper<T, N, N2>>;
      
    private :
      VariantMap	_components;
      
    public :
      UniSoulSystemWrapper(SocketServer&&);
      virtual ~UniSoulSystemWrapper();
      virtual VariantMap& getContent();
    };

    template <typename T, int N, int N2>
    UniSoulSystemWrapper<T, N, N2>
    ::UniSoulSystemWrapper(SocketServer&& socketServer)
    {
      _components["SocketServer"] = std::move(socketServer);
      _components["SocketManager"] = SocketManager();
      _components["PersistentDataInteractor"] = std::make_shared
	<Persistence::PersistentDataFileInteractor>();
      _components["ChannelSystem"] = std::make_shared
	<Communication::UniSoulChannelSystem>();
      _components["CommandFactory"] = CommandFactory();
      _components["CommandExecutor"] = CommandExecutor();
    }

    template <typename T, int N, int N2>
    UniSoulSystemWrapper<T, N, N2>::~UniSoulSystemWrapper() { }

    template <typename T, int N, int N2>
    VariantMap& UniSoulSystemWrapper<T, N, N2>::getContent() { return _components; }
  }
}

template <typename T, int N, int N2>
using UniSoulSystem = Wrapper::System::UniSoulSystemWrapper<T, N, N2>;

#endif /* !UNI_SOUL_SYSTEM_WRAPPER_HPP_ */
