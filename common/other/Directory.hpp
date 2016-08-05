#ifndef DIRECTORY_HPP_
# define DIRECTORY_HPP_

# include <memory>
# include <algorithm>
# include <functional>
# include <vector>

# include "AFile.hpp"

namespace Persistence
{
  namespace File
  {
    template <typename T>
    class Directory;

    template <>
    class Directory<bool> : public AFile<bool>
    {
    private :
      std::vector<FilePtr<bool>>	_filesPtr;

    public :
      Directory(const std::string&);
      virtual ~Directory() = default;
      virtual bool find(const std::string&) const;
      void addAbstractFile(const FilePtr<bool>&);
    };
  
    template <>
    class Directory<std::vector<std::string>> :
      public AFile<std::vector<std::string>>
    {
    private :
      std::vector<FilePtr<std::vector<std::string>>>	_filesPtr;

    public :
      Directory(const std::string&);
      virtual ~Directory() = default;
      virtual std::vector<std::string> find(const std::string& = "") const;
      void addAbstractFile(const FilePtr<std::vector<std::string>>&);
    };

    Directory<bool>::Directory(const std::string& fullName) :
      AFile<bool>(fullName) { }

    bool Directory<bool>::find(const std::string& data) const
    { 
      return std::find_if(_filesPtr.cbegin(),
			  _filesPtr.cend(),
			  [&data](const FilePtr<bool>& filePtr)-> bool
			  {
			    return filePtr->find(data);
			  }) != _filesPtr.cend();
    }
    
    void Directory<bool>::addAbstractFile(const FilePtr<bool>& filePtr)
    {
      _filesPtr.push_back(std::move(filePtr));
    }

    Directory<std::vector<std::string>>
    ::Directory(const std::string& fullName) :
      AFile(fullName)
    {
    }

    std::vector<std::string> Directory<std::vector<std::string>>
    ::find(const std::string&) const
    {
      std::vector<std::string>	datas;
      
      std::for_each(_filesPtr.cbegin(),
		    _filesPtr.cend(),
		    [&datas](const FilePtr<std::vector<std::string>>& filePtr)
		    -> void
		    {
		      datas.merge(filePtr->find());
		    });
      return datas;
    }

    void Directory<std::vector<std::string>>
    ::addAbstractFile(const FilePtr<std::vector<std::string>>& filePtr)
    {
      _filesPtr.push_back(std::move(filePtr));
    }  
  }
}

#endif /* !DIRECTORY_HPP_ */
