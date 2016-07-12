#ifndef UNI_SOUL_SYSTEM_WRAPPER_HH_
# define UNI_SOUL_SYSTEM_WRAPPER_HH_

# include <memory>
# include "ISystemWrapper.hh"
# include "SocketManager.hpp"
# include "CommandFactory.hpp"
# include "CommandExecutor.hpp"

namespace Network
{
  template <typename T>
  class ITCPSocketServer;

  class ITCPSocket;
}

namespace Persistence
{
  class IPersistentDataInteractor;
}

namespace Communication
{
  class IChannelSystem;
}

namespace Wrapper
{
  namespace System
  {
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
	::CommandFactory<UniSoulSystemWrapper>;
      using CommandExecutor = Command
	::CommandExecutor<UniSoulSystemWrapper>;
      
    private :
      VariantMap	_components;
      
    public :
      UniSoulSystemWrapper(const SocketServer&&);
      virtual ~UniSoulSystemWrapper();
      virtual VariantMap& getContent();
    };
  }
}

using UniSoulSystemWrapper = Wrapper::System::UniSoulSystemWrapper;

#endif /* !UNI_SOUL_SYSTEM_WRAPPER_HH_ */
