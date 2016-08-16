#include <iostream>
#include <memory>

#include "UniSoulClient.hpp"
#include "UniSoulConsoleController.hh"
#include "UniSoulConsoleView.hh"

int main()
{
  constexpr const char			HOSTNAME[] = "127.0.0.1";
  constexpr const unsigned short	PORT = 4242;
  
  std::shared_ptr<Model::IModel<View::ViewState>>		modelPtr =
    std::make_shared<Model::UniSoulClient<View::ViewState>>(HOSTNAME, PORT);
  std::shared_ptr<Controller::AController<View::ViewState>>	controllerPtr =
    std::make_shared<Controller::UniSoulConsoleController>(modelPtr);
  std::shared_ptr<View::AView<View::ViewState>>			viewPtr =
    std::make_shared<View::UniSoulConsoleView>(controllerPtr);
  
  modelPtr->addObserver(viewPtr);
  try
    {
      viewPtr->init();
      viewPtr->launch();
      viewPtr->close();
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
      std::cerr << "Unknown exception has been thrown." << std::endl;
    }
  return 1;
}
