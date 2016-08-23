#ifndef UNI_SOUL_CLIENT_HPP_
# define UNI_SOUL_CLIENT_HPP_

# include <memory>
# include <functional>

# if defined(linux) || defined(__linux)
#  include "Epoll.hpp"
#  include "TCPSocketClientLinux.hpp"
#  include "TCPSocketServerLinux.hpp"
# else
#  error "Can't config client. Unknown operating system."
# endif

# include "RemoteConnectionInfo.hh"
# include "CommandFactory.hpp"
# include "CommandExecutor.hpp"
# include "AppState.hh"
# include "PersistentDataFileInteractor.hpp"
# include "Parser.hh"
# include "File.hpp"
# include "UniSoulPacketFactory.hh"
# include "TCPCallbackRead.hh"
# include "TCPCallbackWrite.hh"
# include "TCPCallbackAccept.hh"
# include "HelpCommand.hpp"
# include "QuitCommand.hpp"
# include "CreateConnectionCommand.hpp"
# include "DisconnectCommand.hpp"
# include "MessageCommand.hpp"
# include "StatusCommand.hpp"
# include "GetUserFromCommand.hpp"
# include "IModel.hpp"

namespace Model
{
  template <typename T>
  class UniSoulClient : public IModel<T>
  {
    using MultiplexerPtr = std::unique_ptr<Network::IMultiplexer>;
    
    using TCPSocketServerPtr =
      std::shared_ptr<Network::ITCPSocketServer
		      <int, std::shared_ptr<Network::ITCPSocket<int>>>>;
    
    using ParsedInputs = std::vector<Parser::ParsedInput>;
    using PacketFactory = Factory::UniSoulPacketFactory;
    using DataFileInteractor =
      Persistence::File::PersistentDataFileInteractor
      <std::vector<std::string>>;

  private :
    MultiplexerPtr		_multiplexerPtr;
    TCPSocketServerPtr		_socketServerPtr;
    DataFileInteractor		_dataFileInteractor;
    PacketFactory		_packetFactory;

    Command::CommandFactory
    <std::string,
     App::State,
     MultiplexerPtr,
     TCPSocketServerPtr,
     DataFileInteractor,
     PacketFactory,
     ParsedInputs,
     std::string>		_commandFactory;
    
    Command::CommandExecutor
    <App::State,
     MultiplexerPtr,
     TCPSocketServerPtr,
     DataFileInteractor,
     PacketFactory,
     ParsedInputs,
     std::string>		_commandExecutor;
    
  public :
    UniSoulClient(const std::string&, unsigned short, const std::string&);
    virtual ~UniSoulClient() = default;
    virtual void execute(const ParsedInputs&);
  };

  template <typename T>
  UniSoulClient<T>::UniSoulClient(const std::string& hostname,
				  unsigned short port,
				  const std::string& filename) :
    _dataFileInteractor(std::make_shared
			<Persistence::File::File<std::vector<std::string>>>
			("common/user/" + filename))
  { 
#if defined(linux) || defined(__linux)
    _multiplexerPtr = std::make_unique<Network::Epoll<400, 42>>
      (&Network::TCPCallbackRead::read, &Network::TCPCallbackWrite::write);
    _socketServerPtr
      = std::make_shared<Network::TCPSocketServerLinux<>>
      (hostname, port, Network::RemoteConnectionInfo("Listening endpoint"));
    
    /*_socketPtr["VoIP"] =
      std::make_unique<Network::UDPSocketClientLinux<128>>();*/
#else
# error "Can't config Client. Unknown operating system."
#endif

    _socketServerPtr->open();
    _socketServerPtr->bind();
    _socketServerPtr->listen();
    _multiplexerPtr->addSocket(_socketServerPtr,
			       &Network::TCPCallbackAccept::accept);
    _commandFactory.addCommand("Help",
			       std::make_shared<Command::HelpCommand
			       <App::State,
			       MultiplexerPtr,
			       TCPSocketServerPtr,
			       DataFileInteractor,
			       PacketFactory,
			       ParsedInputs,
			       std::string>>());
    _commandFactory.addCommand("Quit",
			       std::make_shared<Command::QuitCommand
			       <App::State,
			       MultiplexerPtr,
			       TCPSocketServerPtr,
			       DataFileInteractor,
			       PacketFactory,
			       ParsedInputs,
			       std::string>>());
    _commandFactory.addCommand("Disconnect",
			       std::make_shared<Command::DisconnectCommand
			       <App::State,
			       MultiplexerPtr,
			       TCPSocketServerPtr,
			       DataFileInteractor,
			       PacketFactory,
			       ParsedInputs,
			       std::string>>());
    _commandFactory.addCommand("Message",
			       std::make_shared<Command::MessageCommand
			       <App::State,
			       MultiplexerPtr,
			       TCPSocketServerPtr,
			       DataFileInteractor,
			       PacketFactory,
			       ParsedInputs,
			       std::string>>());
    _commandFactory.addCommand("Status",
			       std::make_shared<Command::StatusCommand
			       <App::State,
			       MultiplexerPtr,
			       TCPSocketServerPtr,
			       DataFileInteractor,
			       PacketFactory,
			       ParsedInputs,
			       std::string>>());
    _commandFactory.addCommand("CreateConnection",
			       std::make_shared
			       <Command::CreateConnectionCommand
			       <App::State,
			       MultiplexerPtr,
			       TCPSocketServerPtr,
			       DataFileInteractor,
			       PacketFactory,
			       ParsedInputs,
			       std::string>>());
    _commandFactory.addCommand("GetUserFrom",
			       std::make_shared
			       <Command::GetUserFromCommand
			       <App::State,
			       MultiplexerPtr,
			       TCPSocketServerPtr,
			       DataFileInteractor,
			       PacketFactory,
			       ParsedInputs,
			       std::string>>());
  }
  
  template <typename T>
  void
  UniSoulClient<T>::execute(const std::vector<Parser::ParsedInput>&
			    parsedInputArray)
  {
    T	viewState;
    
    try
      {
	while (_multiplexerPtr->process())
	  _multiplexerPtr->execute();
	try
	  {
	    std::string	retMsg;
	    App::State  state;
	    
	    _commandExecutor
	      .setCommandPtr(_commandFactory
			     .getCommand(parsedInputArray[0].what));
	    state = _commandExecutor
	      .execute(_multiplexerPtr,
		       _socketServerPtr,
		       _dataFileInteractor,
		       _packetFactory,
		       const_cast<ParsedInputs&>(parsedInputArray),
		       retMsg);
	    viewState.set(state == App::State::RUNNING, std::move(retMsg));
	  }
	catch (const Exception::Command::UnknownCommand& e)
	  {
	    viewState.set(true, std::move(e.what()));
	  }
	catch (const Exception::Network::ErrorWithConnection& e)
	  {
	    viewState.set(true, std::move(e.what()));
	  }
	catch (const std::system_error& e)
	  {
	    viewState.set(true, std::move(e.what()));
	  }
	while (_multiplexerPtr->process())
	  _multiplexerPtr->execute();
      }
    catch (const std::system_error& e)
      {
	viewState.set(true, std::move(e.what()));
      }
    this->notifyObservers(viewState);
  }
}

#endif /* !UNI_SOUL_CLIENT_HPP_ */

