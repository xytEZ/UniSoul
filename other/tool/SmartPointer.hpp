#ifndef SMART_POINTER_HPP_
# define SMART_POINTER_HPP_

# include <memory>
# include <boost/shared_ptr.hpp>

namespace Tool
{
  namespace SmartPointer
  {
    template <typename T>
    boost::shared_ptr<T> make_boost_ptr_from_std(std::shared_ptr<T>& ptr)
    {
      return boost::shared_ptr<T>
	(ptr.get(), [ptr](const T *) mutable { ptr.reset(); });
    }

    template <typename T>
    std::shared_ptr<T> make_shared_ptr_from_boost(boost::shared_ptr<T>& ptr)
    {
      return std::shared_ptr<T>
	(ptr.get(), [ptr](const T *) mutable { ptr.reset(); });
    }
  }
}

#endif /* !SMART_POINTER_HPP_ */
