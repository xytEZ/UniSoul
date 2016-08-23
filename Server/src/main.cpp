#include <iostream>
#include <memory>
#include <boost/asio.hpp>

#include "ComplementaryFunction.hpp"
#include "UniSoulServer.hpp"

int main(int argc, char **argv)
{
  if (argc == 3)
    {
      std::unique_ptr<App::IApp>		serverPtr =
	std::make_unique<App::UniSoulServer<boost::asio::io_service>>
	(argv[1], Tool::convert_string_to<unsigned short>(argv[2]));
  
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
    }
  else
    std::cerr << "Usage : ./server [ADDRESS] [PORT]" << std::endl;
  return 1;
}
