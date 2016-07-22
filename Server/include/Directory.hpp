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
    class Directory<StringArray> : public AFile<StringArray>
    {
    private :
      std::vector<FilePtr<StringList>>	_filesPtr;

    public :
      Directory(const std::string&);
      virtual ~Directory() = default;
      virtual StringArray find(const std::string& = "") const;
      void addAbstractFile(const FilePtr<StringArray>&);
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

    Directory<StringArray>::Directory(const std::string& fullName) :
      AFile<StringArray>(fullName)
    {
    }

    StringArray Directory<StringArray>::find(const std::string&) const
    {
      StringArray	datas;
      
      std::for_each(_filesPtr.cbegin(),
		    _filesPtr.cend(),
		    [&datas](const FilePtr<StringList>& filePtr) -> void
		    {
		      datas.merge(filePtr->find());
		    });
      return datas;
    }

    void Directory<StringArray>
    ::addAbstractFile(const FilePtr<StringArray>& filePtr)
    {
      _filesPtr.push_back(std::move(filePtr));
    }  
  }
}

#endif /* !DIRECTORY_HPP_ */
