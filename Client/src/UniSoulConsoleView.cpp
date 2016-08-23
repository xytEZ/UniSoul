#include <iostream>
#include <utility>

#include "UniSoulConsoleView.hh"

namespace View
{
  void ViewState::set(bool start, const std::string& text)
  {
    this->start = start;
    this->text = std::move(text);
  }
  
  UniSoulConsoleView::UniSoulConsoleView(const ControllerPtr& controllerPtr) :
    AView(controllerPtr),
    _viewState { true, "" }
  {
  }
  
  void UniSoulConsoleView::init()
  {
    std::cout << "Welcome to UniSoul client." << std::endl; 
  }
  
  void UniSoulConsoleView::launch()
  {
    std::string	input;
    
    while (_viewState.start)
      {
	std::cout << PROMPT_MESSAGE;
	std::getline(std::cin, input);
	_controllerPtr->performAction(input);
      }
  }
  
  void UniSoulConsoleView::display()
  {
    std::cout << _viewState.text;
  }
  
  void UniSoulConsoleView::close()
  {
    std::cout << "Thank you for using UniSoul client. Good bye." << std::endl;
  }

  void UniSoulConsoleView::update(const ViewState& viewState)
  {
    _viewState = std::move(viewState);
    if (_viewState.start)
      display();
  }
}
