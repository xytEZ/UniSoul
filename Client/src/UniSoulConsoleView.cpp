#include <iostream>
#include <utility>

#include "UniSoulConsoleView.hh"

namespace View
{
  UniSoulConsoleView::UniSoulConsoleView(const ControllerPtr& controllerPtr) :
    AView(controllerPtr),
    _oRes { true, "" }
  {
  }
  
  void UniSoulConsoleView::init()
  {
    std::cout << "Welcome to UniSoul client." << std::endl; 
  }
  
  void UniSoulConsoleView::launch()
  {
    std::string	input;
    
    while (_oRes.start)
      {
	std::cout << PROMPT_MESSAGE;
	std::getline(std::cin, input);
	_controllerPtr->performAction(input);
      }
  }

  void UniSoulConsoleView::display()
  {
    std::cout << _oRes.text << std::endl;
  }

  void UniSoulConsoleView::close()
  {
    std::cout << "Thank you for using UniSoul client. Good bye." << std::endl;
  }

  void UniSoulConsoleView::update(const Observer::OutputResult& arg)
  {
    _oRes = arg;
    display();
  }
}
