#include <iostream>
#include <memory>
#include "UniSoulServer.hh"

int main()
{
  std::unique_ptr<App::IApp>	server
    = std::make_unique<App::UniSoulServer>();

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
