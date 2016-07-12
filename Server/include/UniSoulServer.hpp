#ifndef UNI_SOUL_SERVER_HPP_
# define UNI_SOUL_SERVER_HPP_

# include <memory>
# include "IApp.hh"
# include "BoostServiceWrapper.hh"
# include "TCPBoostSocketServer.hpp"
# include "UniSoulSystemWrapper.hh"

namespace App
{
  template <typename T, int N = 128, int N2 = 60>
  class UniSoulServer : public IApp
  {    
  private :
    using LibraryServiceWrapper = std::unique_ptr<Wrapper::IWrapper<T>>;
    using SystemWrapper = std::unique_ptr<Wrapper::System::ISystemWrapper>;

  private :
    LibraryServiceWrapper	_libraryServiceWrapper;
    SystemWrapper		_systemWrapper;
    
  public :
    UniSoulServer(const std::string&, int);
    virtual ~UniSoulServer();
    virtual bool init();
    virtual bool run();
    virtual bool close();
  };

  template <typename T, int N, int N2>
  UniSoulServer<T, N, N2>::UniSoulServer(const std::string& hostname,
					 int port) :
    _libraryServiceWrapper(std::make_unique<Wrapper::BoostServiceWrapper>()),
    _systemWrapper(std::make_unique
		   <Wrapper::System::UniSoulSystemWrapper>
		   (std::make_shared
		    <Network::TCPBoostSocketServer
		    <N, N2, std::shared_ptr<Network::ITCPSocket>>>
		    (_libraryServiceWrapper->getContent(),
		     _systemWrapper,
		     hostname,
		     port)))
  {
  }
  
  template <typename T, int N, int N2>
  UniSoulServer<T, N, N2>::~UniSoulServer() { }
  
  template <typename T, int N, int N2>
  bool UniSoulServer<T, N, N2>::init()
  {
    boost::any_cast
      <typename UniSoulSystemWrapper::SocketServer>
      (_systemWrapper->getContent()["SocketServer"])->open(0, 0, 0);
    boost::any_cast
      <typename UniSoulSystemWrapper::SocketServer>
      (_systemWrapper->getContent()["SocketServer"])->bind(nullptr, 0);
    boost::any_cast
      <typename UniSoulSystemWrapper::SocketServer>
      (_systemWrapper->getContent()["SocketServer"])->listen(0);
    return true;
  }
  
  template <typename T, int N, int N2>
  bool UniSoulServer<T, N, N2>::run()
  {
    boost::any_cast
      <typename UniSoulSystemWrapper::SocketServer>
      (_systemWrapper->getContent()["SocketServer"])->accept(nullptr, nullptr);
    _libraryServiceWrapper->getContent().run();
    return true;
  }

  template <typename T, int N, int N2>
  bool UniSoulServer<T, N, N2>::close()
  {
    boost::any_cast
      <typename UniSoulSystemWrapper::SocketServer>
      (_systemWrapper->getContent()["SocketServer"])->close();
    boost::any_cast
      <typename UniSoulSystemWrapper::SocketManager>
      (_systemWrapper->getContent()["SocketManager"])
      .apply([](const std::shared_ptr<Network::ITCPSocket>& socket)
	     {
		socket->close();
	     });
    return true;
  }
}

#endif /* !UNI_SOUL_SERVER_HPP_ */
