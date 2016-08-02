#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include "BoostServiceWrapper.hh"
#include "UniSoulServer.hpp"

namespace
{
  using ServerPtr = std::unique_ptr<App::IApp>;

  constexpr const char			HOSTNAME[] = "127.0.0.1";
  constexpr const unsigned short	PORT = 4242;
}

int main()
{
  ServerPtr	serverPtr =
    std::make_unique<App::UniSoulServer<boost::asio::io_service>>
    (HOSTNAME, PORT);
  
  try
    {
      serverPtr->init();
      serverPtr->run();
      serverPtr->close();
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
