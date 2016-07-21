#ifndef ICOMMAND_HPP_
# define ICOMMAND_HPP_

# include <memory>

namespace Command
{
  template <typename T, typename U, typename V>
  class ICommand
  {
  public :
    virtual ~ICommand() = default;
    virtual T execute(U&, const V&) const = 0;
  };

  template <typename T, typename U, typename V>
  using CommandPtr = std::shared_ptr<ICommand<T, U, V>>;
}

#endif /* !ICOMMAND_HPP_ */
