#ifndef UNI_SOUL_SERVER_HPP_
# define UNI_SOUL_SERVER_HPP_

# include <cstddef>
# include <memory>
# include <map>
# include <string>
# include <boost/any.hpp>

# include "IApp.hh"
# include "TCPBoostSocketServer.hpp"

namespace App
{
  template <typename T>
  class UniSoulServer : public IApp
  {    
  private :
    using SystemWrapperPtr =
      std::unique_ptr<Wrapper::IWrapper<std::map<std::string, boost::any>>>;
    
  private :
    T				_networkService;
    SystemWrapperPtr		_systemWrapperPtr;
    
  public :
    UniSoulServer(const std::string&, unsigned short);
    virtual ~UniSoulServer() = default;
    virtual bool init();
    virtual bool run();
    virtual bool close();
  };

  template <typename T>
  UniSoulServer<T>::UniSoulServer(const std::string& hostname,
				  unsigned short port) :
    _systemWrapperPtr(std::make_unique
		      <Wrapper::UniSoulSystemWrapper>
		      (Network::TCPBoostSocketServer<>
		       ::template create<>
		       (_networkService,
			_systemWrapperPtr,
			hostname,
			port)))
  {
  }
  
  template <typename T>
  bool UniSoulServer<T>::init()
  {
    boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::ServerSocketPtr&>
      (_systemWrapperPtr->getContent()["ServerSocket"])->open();
    boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::ServerSocketPtr&>
      (_systemWrapperPtr->getContent()["ServerSocket"])->bind();
    boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::ServerSocketPtr&>
      (_systemWrapperPtr->getContent()["ServerSocket"])->listen();
    return true;
  }
  
  template <typename T>
  bool UniSoulServer<T>::run()
  {
    boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::ServerSocketPtr&>
      (_systemWrapperPtr->getContent()["ServerSocket"])->accept();
    _networkService.run();
    return true;
  }

  template <typename T>
  bool UniSoulServer<T>::close()
  {
    boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::ServerSocketPtr&>
      (_systemWrapperPtr->getContent()["ServerSocket"])->close();
    boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::SocketManager&>
      (_systemWrapperPtr->getContent()["SocketManager"])
      .apply([](const std::pair<std::string, std::shared_ptr
		<Network::ISocket<boost::asio::ip::tcp::socket>>>&
		pairSocketPtr) -> void
	     {
	       pairSocketPtr.second->close();
	     });
    return true;
  }
}

#endif /* !UNI_SOUL_SERVER_HPP_ */
