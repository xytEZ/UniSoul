#include <iostream>
#include "UniSoulConsoleView.hh"

namespace View
{
  UniSoulConsoleView::UniSoulConsoleView(const ControllerPtr& controllerPtr) :
    AView(controllerPtr),
    _start(true),
    _text("")
  {
  }
  
  void UniSoulConsoleView::init()
  {
    std::cout << "Welcome to UniSoul client." << std::endl; 
  }
  
  void UniSoulConsoleView::launch()
  {
    std::string	input;
    
    while (_start)
      {
	std::cout << PROMPT_MESSAGE;
	std::getline(std::cin, input);
	_controllerPtr->performAction(input);
      }
  }

  void UniSoulConsoleView::display()
  {
    std::cout << _text << std::endl;
  }

  void UniSoulConsoleView::close()
  {
    std::cout << "Thank you for using UniSoul client. Good bye." << std::endl;
  }

  void UniSoulConsoleView::update(const Observer::OutputResult& arg)
  {
    _start = arg.start;
    _text = arg.msg;
    display();
  }
}
