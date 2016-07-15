#ifndef FILE_HPP_
# define FILE_HPP_

# include <fstream>
# include "ErrorOpeningFileException.hh"
# include "AFile.hpp"

namespace Persistence
{
  namespace File
  {
    template <typename T>
    class File;

    template <>
    class File<bool> : public AFile<bool>
    {
    public :
      File(const std::string&);
      virtual ~File();
      virtual bool find(const std::string&) const;
    };
  
    template <>
    class File<StringList> : public AFile<StringList>
    { 
    public :
      File(const std::string&);
      virtual ~File();
      virtual StringList find(const std::string& = "") const;
    };

    File<bool>::File(const std::string& fullName) : AFile<bool>(fullName) { }

    File<bool>::~File() { }

    bool File<bool>::find(const std::string& data) const
    {
      std::ifstream	ifs(_fullName, std::ifstream::in);
      bool		isFound = false;

      if (!ifs.is_open())
	throw Exception::Persistence
	  ::ErrorOpeningFileException("Fail to open \""
				      + _fullName
				      + "\" file");
      for (std::string line; ifs >> line;)
	{
	  if (line == data)
	    {
	      isFound = true;
	      break;
	    }
	}
      ifs.close();
      return isFound;
    }

    File<StringList>::File(const std::string& fullName) :
      AFile<StringList>(fullName)
    {
    }

    File<StringList>::~File() { }

    StringList File<StringList>::find(const std::string&) const
    {
      std::ifstream	ifs(_fullName, std::ifstream::in);

      if (!ifs.is_open())
	throw Exception::Persistence
	  ::ErrorOpeningFileException("Fail to open \""
				      + _fullName
				      + "\" file");

      StringList	datas;

      for (std::string line; ifs >> line;)
	datas.push_back(line);
      return datas;
    }
  }
}

#endif /* !FILE_HPP_ */
