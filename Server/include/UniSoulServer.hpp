#ifndef UNI_SOUL_SERVER_HPP_
# define UNI_SOUL_SERVER_HPP_

# include <cstddef>
# include <memory>
# include <map>
# include <string>
# include <boost/any.hpp>
# include "IApp.hh"
# include "BoostServiceWrapper.hh"
# include "TCPBoostSocketServer.hpp"
# include "Descriptor.hh"

namespace App
{
  template <typename T, std::size_t N = 128, std::size_t N2 = 5>
  class UniSoulServer : public IApp
  {    
  private :
    using LibraryServiceWrapperPtr = std::unique_ptr<Wrapper::IWrapper<T>>;
    using SystemWrapperPtr =
      std::unique_ptr<Wrapper::IWrapper<std::map<std::string, boost::any>>>;

  private :
    LibraryServiceWrapperPtr	_libraryServiceWrapperPtr;
    SystemWrapperPtr		_systemWrapperPtr;
    
  public :
    UniSoulServer(const std::string&, unsigned short);
    virtual ~UniSoulServer() = default;
    virtual bool init();
    virtual bool run();
    virtual bool close();
  };

  template <typename T, std::size_t N, std::size_t N2>
  UniSoulServer<T, N, N2>::UniSoulServer(const std::string& hostname,
					    unsigned short port) :
    _libraryServiceWrapperPtr(std::make_unique
			      <Wrapper::BoostServiceWrapper>()),
    _systemWrapperPtr(std::make_unique
		      <Wrapper::UniSoulSystemWrapper>
		      (Network::TCPBoostSocketServer
		       <N, N2, std::shared_ptr
		       <Network::ITCPSocket<::Descriptor>>>
		       ::template create<N, N2, std::shared_ptr
		       <Network::ITCPSocket<::Descriptor>>>
		       (_libraryServiceWrapperPtr->getContent(),
			_systemWrapperPtr,
			hostname,
			port)))
  {
  }
  
  template <typename T, std::size_t N, std::size_t N2>
  bool UniSoulServer<T, N, N2>::init()
  {
    boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::ServerSocketPtr&>
      (_systemWrapperPtr->getContent()["ServerSocket"])->open(0, 0, 0);
    boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::ServerSocketPtr&>
      (_systemWrapperPtr->getContent()["ServerSocket"])->bind(nullptr, 0);
    boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::ServerSocketPtr&>
      (_systemWrapperPtr->getContent()["ServerSocket"])->listen(0);
    return true;
  }
  
  template <typename T, std::size_t N, std::size_t N2>
  bool UniSoulServer<T, N, N2>::run()
  {
    boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::ServerSocketPtr&>
      (_systemWrapperPtr->getContent()["ServerSocket"])
      ->accept(nullptr, nullptr);
    _libraryServiceWrapperPtr->getContent().run();
    return true;
  }

  template <typename T, std::size_t N, std::size_t N2>
  bool UniSoulServer<T, N, N2>::close()
  {
    boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::ServerSocketPtr&>
      (_systemWrapperPtr->getContent()["ServerSocket"])->close();
    boost::any_cast
      <typename Wrapper::UniSoulSystemWrapper::ConnectionManager&>
      (_systemWrapperPtr->getContent()["ConnectionManager"])
      .apply([](const std::shared_ptr
		<Network::TCPConnection
		<Info::ClientInfo, ::Descriptor>>& connectionPtr) -> void
	     {
	       connectionPtr->getSocketPtr()->close();
	     });
    return true;
  }
}

#endif /* !UNI_SOUL_SERVER_HPP_ */
