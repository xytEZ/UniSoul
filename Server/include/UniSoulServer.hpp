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

namespace App
{
  template <typename T, std::size_t N = 128, std::size_t N2 = 60>
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
    UniSoulServer(const std::string&, int);
    virtual ~UniSoulServer();
    virtual bool init();
    virtual bool run();
    virtual bool close();
  };

  template <typename T, std::size_t N, std::size_t N2>
  UniSoulServer<T, N, N2>::UniSoulServer(const std::string& hostname,
					 int port) :
    _libraryServiceWrapperPtr(std::make_unique
			      <Wrapper::BoostServiceWrapper>()),
    _systemWrapperPtr(std::make_unique
		      <Wrapper::UniSoulSystemWrapper>
		      (Network::TCPBoostSocketServer
		       <N, N2, std::shared_ptr<Network::ITCPSocket>>
		       ::template create<N, N2, std::shared_ptr
		       <Network::ITCPSocket>>
		       (_libraryServiceWrapperPtr->getContent(),
			_systemWrapperPtr,
			hostname,
			port)))
  {
  }
  
  template <typename T, std::size_t N, std::size_t N2>
  UniSoulServer<T, N, N2>::~UniSoulServer() { }
  
  template <typename T, std::size_t N, std::size_t N2>
  bool UniSoulServer<T, N, N2>::init()
  {
    boost::any_cast
      <typename UniSoulSystemWrapper::ServerSocketPtr&>
      (_systemWrapperPtr->getContent()["ServerSocket"])->open(0, 0, 0);
    boost::any_cast
      <typename UniSoulSystemWrapper::ServerSocketPtr&>
      (_systemWrapperPtr->getContent()["ServerSocket"])->bind(nullptr, 0);
    boost::any_cast
      <typename UniSoulSystemWrapper::ServerSocketPtr&>
      (_systemWrapperPtr->getContent()["ServerSocket"])->listen(0);
    return true;
  }
  
  template <typename T, std::size_t N, std::size_t N2>
  bool UniSoulServer<T, N, N2>::run()
  {
    boost::any_cast
      <typename UniSoulSystemWrapper::ServerSocketPtr&>
      (_systemWrapperPtr->getContent()["ServerSocket"])->accept(nullptr,
								nullptr);
    _libraryServiceWrapperPtr->getContent().run();
    return true;
  }

  template <typename T, std::size_t N, std::size_t N2>
  bool UniSoulServer<T, N, N2>::close()
  {
    boost::any_cast
      <typename UniSoulSystemWrapper::ServerSocketPtr&>
      (_systemWrapperPtr->getContent()["ServerSocket"])->close();
    return true;
  }
}

#endif /* !UNI_SOUL_SERVER_HPP_ */
