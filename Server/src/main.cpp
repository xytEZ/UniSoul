#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include "BoostServiceWrapper.hh"
#include "UniSoulServer.hpp"

using Server = std::unique_ptr<App::IApp>;

int main()
{
  constexpr const char	hostname[] = "127.0.0.1";
  constexpr const int	port = 4242;
  Server		server
    = std::make_unique
    <App::UniSoulServer<boost::asio::io_service>>(hostname, port);
  
  try
    {
      server->init();
      server->run();
      server->close();
    }
  catch (const std::exception& e)
    {
      std::cerr << "Exception has been thrown. Error message is :"
		<< std::endl
		<< e.what()
		<< std::endl;
    }
  catch (...)
    {
      std::cerr << "Unknown exception has been thrown" << std::endl;
    }
  return 1;
}
