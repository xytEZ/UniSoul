#include <iostream>
#include <memory>

#include "ComplementaryFunction.hpp"
#include "UniSoulClient.hpp"
#include "UniSoulConsoleController.hh"
#include "UniSoulConsoleView.hh"

int main(int argc, char **argv)
{
  if (argc == 4)
    {
      std::shared_ptr<Model::IModel<View::ViewState>>		modelPtr =
	std::make_shared<Model::UniSoulClient<View::ViewState>>
	(argv[1], Tool::convert_string_to<unsigned short>(argv[2]), argv[3]);
	 
      
      std::shared_ptr<Controller::AController<View::ViewState>>	controllerPtr =
	std::make_shared<Controller::UniSoulConsoleController>
	(modelPtr);
      
      std::shared_ptr<View::AView<View::ViewState>>		viewPtr =
	std::make_shared<View::UniSoulConsoleView>
	(controllerPtr);
  
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
    }
  else
    std::cerr << "Usage : ./client [ADDRESS] [PORT] [FILENAME]" << std::endl;
  return 1;
}
