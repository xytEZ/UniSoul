#ifndef IMODEL_HPP_
# define IMODEL_HPP_

# include <string>
# include "Observable.hpp"

namespace Model
{
  template <typename T>
  class IModel : public Observer::Observable<T>
  {
  public :
    virtual ~IModel() = default;
    virtual void execute(const std::string&) = 0;
  };
}

#endif /* !IMODEL_HPP_ */
