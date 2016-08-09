#include <iostream>
#include <memory>

#include "ObserverOutputResult.hh"
#include "UniSoulClient.hh"
#include "UniSoulConsoleController.hh"
#include "UniSoulConsoleView.hh"

namespace
{
  template <typename T>
  using ModelPtr = std::shared_ptr<Model::IModel<T>>;

  template <typename T>
  using ControllerPtr = std::shared_ptr<Controller::AController<T>>;

  template <typename T>
  using ViewPtr = std::shared_ptr<View::AView<T>>;
  
  constexpr const char			HOSTNAME[] = "127.0.0.1";
  constexpr const unsigned short	PORT = 4242;
}

int main()
{
  ModelPtr<Observer::OutputResult>	modelPtr =
    std::make_shared<Model::UniSoulClient>(HOSTNAME, PORT);
  ControllerPtr<Observer::OutputResult>	controllerPtr =
    std::make_shared<Controller::UniSoulConsoleController>(modelPtr);
  ViewPtr<Observer::OutputResult>	viewPtr =
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
