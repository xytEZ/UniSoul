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
    using LibraryServiceWrapperPtr = std::unique_ptr<Wrapper::IWrapper<T>>;
    using SystemWrapperPtr = std::unique_ptr<Wrapper::System::ISystemWrapper>;

  private :
    LibraryServiceWrapperPtr	_libraryServiceWrapperPtr;
    SystemWrapperPtr		_systemWrapperPtr;
    
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
    _libraryServiceWrapperPtr(std::make_unique<Wrapper::BoostServiceWrapper>()),
    _systemWrapperPtr(std::make_unique
		      <Wrapper::System::UniSoulSystemWrapper>
		      (Network::TCPBoostSocketServer
		       <N, N2, std::shared_ptr<Network::ITCPSocket>>
		       ::template create<N, N2, std::shared_ptr
		       <Network::ITCPSocket>>(_libraryServiceWrapperPtr->getContent(),
					      _systemWrapperPtr,
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
      <typename UniSoulSystemWrapper::SocketServerPtr>
      (_systemWrapperPtr->getContent()["SocketServer"])->open(0, 0, 0);
    boost::any_cast
      <typename UniSoulSystemWrapper::SocketServerPtr>
      (_systemWrapperPtr->getContent()["SocketServer"])->bind(nullptr, 0);
    boost::any_cast
      <typename UniSoulSystemWrapper::SocketServerPtr>
      (_systemWrapperPtr->getContent()["SocketServer"])->listen(0);
    return true;
  }
  
  template <typename T, int N, int N2>
  bool UniSoulServer<T, N, N2>::run()
  {
    boost::any_cast
      <typename UniSoulSystemWrapper::SocketServerPtr>
      (_systemWrapperPtr->getContent()["SocketServer"])->accept(nullptr, nullptr);
    _libraryServiceWrapperPtr->getContent().run();
    return true;
  }

  template <typename T, int N, int N2>
  bool UniSoulServer<T, N, N2>::close()
  {
    boost::any_cast
      <typename UniSoulSystemWrapper::SocketServerPtr>
      (_systemWrapperPtr->getContent()["SocketServer"])->close();
    boost::any_cast
      <typename UniSoulSystemWrapper::SocketManager>
      (_systemWrapperPtr->getContent()["SocketManager"])
      .apply([](const std::shared_ptr<Network::ITCPSocket>& socket)
	     {
	       socket->close();
	     });
    return true;
  }
}

#endif /* !UNI_SOUL_SERVER_HPP_ */
