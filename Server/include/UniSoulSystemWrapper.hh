#ifndef UNI_SOUL_SYSTEM_WRAPPER_HH_
# define UNI_SOUL_SYSTEM_WRAPPER_HH_

# include <memory>
# include "ISystemWrapper.hh"
# include "SocketManager.hpp"
# include "CommandFactory.hpp"
# include "CommandExecutor.hpp"
# include "UniSoulChannelSystem.hh"
# include "UserCheckerManager.hh"

namespace Network
{
  template <typename T>
  class ITCPSocketServer;

  class ITCPSocket;
}

namespace Wrapper
{
  namespace System
  {
    class UniSoulSystemWrapper : public ISystemWrapper
    { 
    public :
      using SocketServerPtr = std::shared_ptr
	<Network::ITCPSocketServer
	 <std::shared_ptr<Network::ITCPSocket>>>;
      using SocketManager = Network::Manager::SocketManager
	<std::shared_ptr<Network::ITCPSocket>>;
      using UserCheckerManager = Persistence::Manager::UserCheckerManager;
      using ChannelSystem = Communication::Channel::UniSoulChannelSystem;
      using CommandFactory = Command
	::CommandFactory<UniSoulSystemWrapper>;
      using CommandExecutor = Command
	::CommandExecutor<UniSoulSystemWrapper>;
      
    private :
      VariantMap	_components;
      
    public :
      UniSoulSystemWrapper(SocketServerPtr&&);
      virtual ~UniSoulSystemWrapper();
      virtual VariantMap& getContent();
    };
  }
}

using UniSoulSystemWrapper = Wrapper::System::UniSoulSystemWrapper;

#endif /* !UNI_SOUL_SYSTEM_WRAPPER_HH_ */
