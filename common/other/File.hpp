#ifndef FILE_HPP_
# define FILE_HPP_

# include <fstream>
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
      virtual ~File() = default;
      virtual bool find(const std::string&) const;
    };
  
    template <>
    class File<StringArray> : public AFile<StringArray>
    { 
    public :
      File(const std::string&);
      virtual ~File() = default;
      virtual StringArray find(const std::string& = "") const;
    };

    File<bool>::File(const std::string& fullName) : AFile<bool>(fullName) { }

    bool File<bool>::find(const std::string& data) const
    {
      std::ifstream	ifs;
      bool		isFound = false;

      ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      try
	{
	  ifs.open(_fullName, std::ifstream::in);
	  for (std::string line; ifs >> line;)
	    {
	      if (line == data)
		{
		  isFound = true;
		  break;
		}
	    }
	}
      catch (const std::ifstream::failure&) { }
      ifs.close();
      return isFound;
    }

    File<StringArray>::File(const std::string& fullName) :
      AFile<StringArray>(fullName)
    {
    }

    StringArray File<StringArray>::find(const std::string&) const
    {
      std::ifstream	ifs;
      StringArray	datas;

      ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      try
	{
	  ifs.open(_fullName, std::ifstream::in);
	  for (std::string line; ifs >> line;)
	    datas.push_back(line);
	}
      catch (const std::ifstream::failure&) { }
      ifs.close();
      return datas;
    }
  }
}

#endif /* !FILE_HPP_ */