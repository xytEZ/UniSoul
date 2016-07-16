#ifndef DIRECTORY_HPP_
# define DIRECTORY_HPP_

# include <memory>
# include <algorithm>
# include <functional>
# include <vector>
# include "ErrorOpeningFileException.hh"
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
      virtual ~Directory();
      virtual bool find(const std::string&) const;
      void addAbstractFile(const FilePtr<bool>&);
    };
  
    template <>
    class Directory<StringList> : public AFile<StringList>
    {
    private :
      std::vector<FilePtr<StringList>>	_filesPtr;

    public :
      Directory(const std::string&);
      virtual ~Directory();
      virtual StringList find(const std::string& = "") const;
      void addAbstractFile(const FilePtr<StringList>&);
    };

    Directory<bool>::Directory(const std::string& fullName) :
      AFile<bool>(fullName) { }

    Directory<bool>::~Directory() { }

    bool Directory<bool>::find(const std::string& data) const
    { 
      return std::find_if(_filesPtr.cbegin(),
			  _filesPtr.cend(),
			  [&data](const FilePtr<bool>& filePtr)-> bool
			  {
			    try
			      {
				return filePtr->find(data);
			      }
			    catch (const Exception::Persistence
				   ::ErrorOpeningFileException& e)
			      {
				return false;
			      }
			  }) != _filesPtr.cend();
    }
    
    void Directory<bool>::addAbstractFile(const FilePtr<bool>& filePtr)
    {
      _filesPtr.push_back(std::move(filePtr));
    }

    Directory<StringList>::Directory(const std::string& fullName) :
      AFile<StringList>(fullName)
    {
    }

    Directory<StringList>::~Directory() { }

    StringList Directory<StringList>::find(const std::string&) const
    {
      StringList	datas;
      
      std::for_each(_filesPtr.cbegin(),
		    _filesPtr.cend(),
		    [&datas](const FilePtr<StringList>& filePtr) -> void
		    {
		      try
			{
			  datas.merge(filePtr->find());
			}
		      catch (const Exception::Persistence
			     ::ErrorOpeningFileException& e) { }
		    });
      return datas;
    }

    void Directory<StringList>
    ::addAbstractFile(const FilePtr<StringList>& filePtr)
    {
      _filesPtr.push_back(std::move(filePtr));
    }  
  }
}

#endif /* !DIRECTORY_HPP_ */
