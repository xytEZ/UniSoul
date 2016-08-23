#ifndef AFILE_HPP_
# define AFILE_HPP_

# include <memory>
# include <string>

namespace Persistence
{
  namespace File
  {
    template <typename T>
    class AFile
    {
    protected :
      std::string	_fullName;
    
    public :
      AFile(const std::string&);
      virtual ~AFile() = default;
      virtual T find(const std::string& = "") const = 0;
    };
    
    template <typename T>
    AFile<T>::AFile(const std::string& fullName) : _fullName(fullName) { }
    
    template <typename T>
    using FilePtr = std::shared_ptr<AFile<T>>;
  }
}

#endif /* !AFILE_HPP_ */
