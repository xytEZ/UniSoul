#include <memory>

#if defined(linux) || defined(__linux)
# include "Epoll.hpp"
# include "TCPSocketClientLinux.hpp"
//# include "UDPSocketClientLinux.hpp"
#else
# error "Can't config client. Unknown operating system."
#endif

#include "File.hpp"
#include "HelpCommand.hpp"
#include "QuitCommand.hpp"
#include "ConnectCommand.hpp"
#include "DeconnectCommand.hpp"
#include "UniSoulClient.hh"

namespace Model
{
  UniSoulClient::UniSoulClient(const std::string& hostname,
			       unsigned short port) :
    _dataFileInteractor(std::make_shared
			<Persistence::File::File<std::vector<std::string>>>
			("../../other/user/user1.txt")),
    _commandFactory(),
    _commandExecutor()
  {
#if defined(linux) || defined(__linux)
    _multiplexerPtr = std::make_unique<Network::Epoll<42>>
      (Network::IMultiplexer::ioCallback(),
       Network::IMultiplexer::ioCallback(),
       60);
    _tcpSocketClientPtr =
      std::make_unique<Network::TCPSocketClientLinux<128>>
      (hostname, port);
    /*_udpSocketClientPtr =
      std::make_unique<Network::UDPSocketClientLinux<128>>();*/
#else
# error "Can't config Client. Unknown operating system."
#endif

    _commandFactory.addCommand("Help",
			       std::make_shared<Command::HelpCommand
			       <App::State::Flag,
			       std::vector<Parser::ParsedInput>,
			       std::string>>());
    _commandFactory.addCommand("Quit",
			       std::make_shared<Command::QuitCommand
			       <App::State::Flag,
			       std::vector<Parser::ParsedInput>,
			       std::string>>());
    _commandFactory.addCommand("Connect",
			       std::make_shared<Command::ConnectCommand
			       <App::State::Flag,
			       std::vector<Parser::ParsedInput>,
			       std::string>>());
    _commandFactory.addCommand("Deconnect",
			       std::make_shared<Command::DeconnectCommand
			       <App::State::Flag,
			       std::vector<Parser::ParsedInput>,
			       std::string>>());
  }

  UniSoulClient::~UniSoulClient()
  {
  }

  void UniSoulClient
  ::execute(const std::vector<Parser::ParsedInput>& parsedInputArray)
  {
    Observer::OutputResult	oRes;
    std::string			retMsg;
    App::State::Flag		state;
    
    try
      {
	_commandExecutor
	  .setCommandPtr(_commandFactory
			 .getCommand(parsedInputArray[0].what));
	state = _commandExecutor
	  .execute(const_cast
		   <std::vector<Parser::ParsedInput>&>(parsedInputArray),
		   retMsg);
	oRes.start = (state == App::State::RUNNING);
	oRes.text = std::move(retMsg);
      }
    catch (const Exception::Command::CommandNotFoundException& e)
      {
	oRes.start = true;
	oRes.text = std::move(e.what());
      }
    notifyObservers(oRes);
  }
}
