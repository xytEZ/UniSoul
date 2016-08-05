#ifndef UNI_SOUL_SYSTEM_WRAPPER_HH_
# define UNI_SOUL_SYSTEM_WRAPPER_HH_

# include <memory>
# include <map>
# include <string>
# include <vector>
# include <utility>
# include <boost/any.hpp>

# include "IWrapper.hpp"
# include "ConnectionManager.hpp"
# include "CommandFactory.hpp"
# include "CommandExecutor.hpp"
# include "ChatRoomManager.hh"
# include "ClientCheckerManager.hh"
# include "ClientInfo.hh"
# include "TCPConnection.hpp"
# include "BoostDescriptor.hh"
# include "CommandType.hh"
# include "ConnectionStateFlag.hh"

namespace Network
{
  template <typename T, typename U>
  class ITCPSocketServer;

  template <typename T>
  class ITCPSocket;
}

namespace Wrapper
{
  using VariantMap = std::map<std::string, boost::any>;

  class UniSoulSystemWrapper : public IWrapper<VariantMap>
  { 
  public :
    using ServerSocketPtr = std::shared_ptr
      <Network::ITCPSocketServer
       <::Descriptor, std::shared_ptr<Network::ITCPSocket<::Descriptor>>>>;
    
    using ConnectionManager = Network::Manager::ConnectionManager
      <std::shared_ptr
       <Network::TCPConnection<Info::ClientInfo, ::Descriptor>>>;
    
    using ClientCheckerManager = Persistence::Manager::ClientCheckerManager;
    using ChatRoomManager = Communication::Chat::ChatRoomManager;
    
    using CommandFactory = Command::CommandFactory
      <Command::Type,
       Network::ConnectionStateFlag,
       std::unique_ptr<IWrapper<VariantMap>>,
       std::vector<std::string>,
       std::string>;
    
    using CommandExecutor = Command::CommandExecutor
      <Network::ConnectionStateFlag,
       std::unique_ptr<IWrapper<VariantMap>>,
       std::vector<std::string>,
       std::string>;
    
  private :
    VariantMap	_components;
      
  public :
    UniSoulSystemWrapper(const ServerSocketPtr&);
    virtual ~UniSoulSystemWrapper() = default;
    virtual VariantMap& getContent();
  };
}

#endif /* !UNI_SOUL_SYSTEM_WRAPPER_HH_ */
