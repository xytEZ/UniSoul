#ifndef UNI_SOUL_CLIENT_HH_
# define UNI_SOUL_CLIENT_HH_

# include <memory>

# include "ObserverOutputResult.hh"
# include "CommandFactory.hpp"
# include "CommandExecutor.hpp"
# include "AppStateFlag.hh"
# include "PersistentDataFileInteractor.hpp"
# include "Parser.hh"
# include "IModel.hpp"

namespace Network
{
  class IMultiplexer;

  template <typename T>
  class ITCPSocketClient;

  /*template <typename T>
    class IUDPSocketClient;*/
}

namespace Model
{
  class UniSoulClient : public IModel<Observer::OutputResult>
  {
  private :
    using MultiplexerPtr = std::unique_ptr<Network::IMultiplexer>;
    using TCPSocketClientPtr = std::shared_ptr<Network::ITCPSocketClient<int>>;
    /*using UDPSocketClientPtr =
      std::shared_ptr<Network::IUDPSocketClient<int>>;*/
    
  private :
    MultiplexerPtr				_multiplexerPtr;
    TCPSocketClientPtr				_tcpSocketClientPtr;
    //UDPSocketClientPtr			_udpSocketClientPtr;

    Persistence::File
    ::PersistentDataFileInteractor
    <std::vector<std::string>>			_dataFileInteractor;
    
    Command::CommandFactory<std::string,
			    App::State::Flag,
			    std::vector
			    <Parser
			     ::ParsedInput>,
			    std::string>	_commandFactory;
    
    Command::CommandExecutor<App::State::Flag,
			     std::vector
			     <Parser
			      ::ParsedInput>,
			     std::string>	_commandExecutor;
    
  public :
    UniSoulClient(const std::string&, unsigned short);
    virtual ~UniSoulClient();
    virtual void execute(const std::vector<Parser::ParsedInput>&);
  };
}

#endif /* !UNI_SOUL_CLIENT_HH_ */
