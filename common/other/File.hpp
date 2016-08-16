#ifndef FILE_HPP_
# define FILE_HPP_

# include <fstream>
# include <vector>

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
    class File<std::vector<std::string>> :
      public AFile<std::vector<std::string>>
    { 
    public :
      File(const std::string&);
      virtual ~File() = default;
      virtual std::vector<std::string> find(const std::string& = "") const;
    };

    File<bool>::File(const std::string& fullName) : AFile<bool>(fullName) { }

    bool File<bool>::find(const std::string& data) const
    {
      std::ifstream	ifs;
      bool		isFound = false;

      try
	{
	  std::string	line;

	  ifs.open(_fullName, std::ifstream::in);
	  while (std::getline(ifs, line))
	    {
	      if (line == data)
		{
		  isFound = true;
		  break;
		}
	    }
	  ifs.close();
	}
      catch (const std::ifstream::failure&)
	{
	  throw;
	}
      return isFound;
    }
    
    File<std::vector<std::string>>::File(const std::string& fullName) :
      AFile(fullName)
    {
    }
    
    std::vector<std::string> File<std::vector<std::string>>
    ::find(const std::string&) const
    {
      std::ifstream		ifs;
      std::vector<std::string>	datas;

      try
	{
	  std::string	line;
	  
	  ifs.open(_fullName, std::ifstream::in);
	  while (std::getline(ifs, line))
	    datas.push_back(line);
	  ifs.close();
	}
      catch (const std::ifstream::failure&)
	{
	  throw;
	}
      return datas;
    }
  }
}

#endif /* !FILE_HPP_ */
