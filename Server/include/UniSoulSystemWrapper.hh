#ifndef UNI_SOUL_SYSTEM_WRAPPER_HH_
# define UNI_SOUL_SYSTEM_WRAPPER_HH_

# include <memory>
# include <map>
# include <string>
# include <vector>
# include <utility>
# include <boost/any.hpp>
# include <boost/asio.hpp>

# include "IWrapper.hpp"
# include "SocketManager.hpp"
# include "CommandFactory.hpp"
# include "CommandExecutor.hpp"
# include "ChatRoom.hh"
# include "ClientChecker.hh"
# include "CommandType.hh"
# include "ConnectionState.hh"

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
       <boost::asio::ip::tcp::socket,
	std::shared_ptr<Network::ITCPSocket<boost::asio::ip::tcp::socket>>>>;
    
    using SocketManager = Network::Manager::SocketManager
      <boost::asio::ip::tcp::socket>;
    
    using ClientChecker = Persistence::ClientChecker;
    using ChatRoomManager = Communication::Chat::ChatRoomManager;
    
    using CommandFactory = Command::CommandFactory
      <Command::Type,
       Network::ConnectionState,
       std::unique_ptr<IWrapper<VariantMap>>,
       std::vector<std::string>,
       std::string>;
    
    using CommandExecutor = Command::CommandExecutor
      <Network::ConnectionState,
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
