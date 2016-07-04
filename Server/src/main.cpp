#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include "UniSoulServer.hpp"

int main()
{
  constexpr char		hostname[] = "aries.kent.ac.uk";
  constexpr int			port = 4242;
  std::unique_ptr<App::IApp>	server
    = std::make_unique<App::UniSoulServer<boost::asio::io_service>>(hostname, port);

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
