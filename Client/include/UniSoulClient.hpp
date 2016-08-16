#ifndef UNI_SOUL_CLIENT_HPP_
# define UNI_SOUL_CLIENT_HPP_

# include <memory>
# include <functional>

# if defined(linux) || defined(__linux)
#  include "Epoll.hpp"
#  include "TCPSocketClientLinux.hpp"
# else
#  error "Can't config client. Unknown operating system."
# endif

# include "CommandFactory.hpp"
# include "CommandExecutor.hpp"
# include "AppStateFlag.hh"
# include "PersistentDataFileInteractor.hpp"
# include "Parser.hh"
# include "File.hpp"
# include "UniSoulPacketFactory.hh"
# include "TCPCallbackRead.hh"
# include "TCPCallbackWrite.hh"
# include "HelpCommand.hpp"
# include "QuitCommand.hpp"
# include "ConnectCommand.hpp"
# include "DisconnectCommand.hpp"
# include "MessageCommand.hpp"
# include "StatusCommand.hpp"
# include "IModel.hpp"

namespace Model
{
  template <typename T>
  class UniSoulClient : public IModel<T>
  {
  private :
    using MultiplexerPtr = std::unique_ptr<Network::IMultiplexer>;
    using SocketPtr = std::shared_ptr<Network::ISocket<int>>;
    using SocketsPtr = std::map<std::string, SocketPtr>;
    using ParsedInputs = std::vector<Parser::ParsedInput>;
    using PacketFactory = Factory::UniSoulPacketFactory;

    using DataFileInteractor =
      Persistence::File::PersistentDataFileInteractor
      <std::vector<std::string>>;

  private :
    MultiplexerPtr		_multiplexerPtr;
    SocketsPtr			_socketsPtr;
    DataFileInteractor		_dataFileInteractor;
    PacketFactory		_packetFactory;

    Command::CommandFactory
    <std::string,
     App::State::Flag,
     MultiplexerPtr,
     SocketsPtr,
     DataFileInteractor,
     PacketFactory,
     ParsedInputs,
     std::string>		_commandFactory;
    
    Command::CommandExecutor
    <App::State::Flag,
     MultiplexerPtr,
     SocketsPtr,
     DataFileInteractor,
     PacketFactory,
     ParsedInputs,
     std::string>		_commandExecutor;
    
  public :
    UniSoulClient(const std::string&, unsigned short);
    virtual ~UniSoulClient() = default;
    virtual void execute(const ParsedInputs&);
  };

  template <typename T>
  UniSoulClient<T>::UniSoulClient(const std::string& hostname,
				  unsigned short port) :
    _dataFileInteractor(std::make_shared
			<Persistence::File::File<std::vector<std::string>>>
			("../../common/user/user1.txt"))
  {
#if defined(linux) || defined(__linux)
    _multiplexerPtr = std::make_unique<Network::Epoll<300, 42>>
      (std::bind(&Network::TCPCallbackRead::read, std::placeholders::_1),
       std::bind(&Network::TCPCallbackWrite::write, std::placeholders::_1));
    _socketsPtr["Server"] =
      std::make_unique<Network::TCPSocketClientLinux<>>(hostname, port);

    /*_socketPtr["Voice"] =
      std::make_unique<Network::UDPSocketClientLinux<128>>();*/
#else
# error "Can't config Client. Unknown operating system."
#endif
    _commandFactory.addCommand("Help",
			       std::make_shared<Command::HelpCommand
			       <App::State::Flag,
			       MultiplexerPtr,
			       SocketsPtr,
			       DataFileInteractor,
			       PacketFactory,
			       ParsedInputs,
			       std::string>>());
    _commandFactory.addCommand("Quit",
			       std::make_shared<Command::QuitCommand
			       <App::State::Flag,
			       MultiplexerPtr,
			       SocketsPtr,
			       DataFileInteractor,
			       PacketFactory,
			       ParsedInputs,
			       std::string>>());
    _commandFactory.addCommand("Connect",
			       std::make_shared<Command::ConnectCommand
			       <App::State::Flag,
			       MultiplexerPtr,
			       SocketsPtr,
			       DataFileInteractor,
			       PacketFactory,
			       ParsedInputs,
			       std::string>>());
    _commandFactory.addCommand("Disconnect",
			       std::make_shared<Command::DisconnectCommand
			       <App::State::Flag,
			       MultiplexerPtr,
			       SocketsPtr,
			       DataFileInteractor,
			       PacketFactory,
			       ParsedInputs,
			       std::string>>());
    _commandFactory.addCommand("Message",
			       std::make_shared<Command::MessageCommand
			       <App::State::Flag,
			       MultiplexerPtr,
			       SocketsPtr,
			       DataFileInteractor,
			       PacketFactory,
			       ParsedInputs,
			       std::string>>());
    _commandFactory.addCommand("Status",
			       std::make_shared<Command::StatusCommand
			       <App::State::Flag,
			       MultiplexerPtr,
			       SocketsPtr,
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
	try
	  {
	    std::string		retMsg;
	    App::State::Flag     state;
	    
	    _commandExecutor
	      .setCommandPtr(_commandFactory
			     .getCommand(parsedInputArray[0].what));
	    state = _commandExecutor
	      .execute(_multiplexerPtr,
		       _socketsPtr,
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
	_multiplexerPtr->process();
	_multiplexerPtr->execute();
      }
    catch (const Exception::Network::ErrorWithConnection& e)
      {
	viewState.set(true, std::move(e.what()));
      }
    catch (const std::system_error& e)
      {
	viewState.set(true, std::move(e.what()));
      }
    this->notifyObservers(viewState);
  }
}

#endif /* !UNI_SOUL_CLIENT_HPP_ */
