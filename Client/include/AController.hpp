#ifndef ACONTROLLER_HPP_
# define ACONTROLLER_HPP_

# include <memory>
# include <string>
# include <utility>
# include "IModel.hpp"

namespace Controller
{
  template <typename T>
  class AController
  {
  protected :
    using ModelPtr = std::shared_ptr<Model::IModel<T>>;
    
  protected :
    ModelPtr	_modelPtr;
    
  protected :
    AController(const ModelPtr&);
    
  public :
    virtual ~AController() = default;
    virtual void performAction(const std::string&) = 0;
  };

  template <typename T>
  AController<T>::AController(const ModelPtr& modelPtr) :
    _modelPtr(modelPtr)
  {
  }
}

#endif /* !ACONTROLLER_HPP_ */
