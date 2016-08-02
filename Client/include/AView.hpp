#ifndef AVIEW_HPP_
# define AVIEW_HPP_

# include <memory>
# include "IObserver.hpp"
# include "AController.hpp"

namespace View
{
  template <typename T>
  class AView : public Observer::IObserver<T>
  {
  protected :
    using ControllerPtr = std::shared_ptr<Controller::AController<T>>;
    
  protected :
    ControllerPtr	_controllerPtr;
    
  protected :
    AView(const ControllerPtr&);

  public :
    virtual ~AView() = default;
    virtual void init() = 0;
    virtual void launch() = 0;
    virtual void display() = 0;
    virtual void close() = 0;
  };

  template <typename T>
  AView<T>::AView(const ControllerPtr& controllerPtr) :
    _controllerPtr(controllerPtr)
  {
  }
}

#endif /* !AVIEW_HPP_ */
