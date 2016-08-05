#ifndef IMODEL_HPP_
# define IMODEL_HPP_

# include <vector>
# include <string>

# include "Parser.hh"
# include "Observable.hpp"

namespace Model
{
  template <typename T>
  class IModel : public Observer::Observable<T>
  {
  public :
    virtual ~IModel() = default;
    virtual void execute(const std::vector<Parser::ParsedInput>&) = 0;
  };
}

#endif /* !IMODEL_HPP_ */
