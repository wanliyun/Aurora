#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include <string>
#include <vector>
#include <boost/serialization/singleton.hpp>

namespace Aurora
{
	//单键模式;
	class  FileSystem : public boost::serialization::singleton<FileSystem>
	{
	public:
		FileSystem():m_bInited(false){}
		~FileSystem(){}

		/** 用文件初始化文件系统;
		 *	@param szInitFilePath 文件相对于exe文件的相对路径或者绝对路径;
		 *	@return 是否成功;
		 *	@note 
		 */
		bool Init(const char * szInitFilePath);

		/** 从相对路径计算出绝对路径来;
		 *	@param szPath 相对路径;
		 *	@return 文件的绝对路径  返回0表示没有找到文件;
		 *	@note 
		 */
		std::string FindFilePath(const std::string & path)const{return FindFilePath(path.c_str());}
		std::string FindFilePath(const char * szPath)const;

		std::string FindDirectoryPath(const std::string & path)const{return FindDirectoryPath(path.c_str());}
		std::string FindDirectoryPath(const char * szPath)const;
	
		size_t		GetNumPaths()const{return m_paths.size();}
		std::string GetPath(size_t i)const{return i >= m_paths.size() ? std::string() : m_paths[i];}

		static bool FileExists(const std::string & filePath){return FileExists(filePath.c_str());}
		static bool FileExists(const char * filePath);

		static bool DirectoryExists(const std::string & filePath){return DirectoryExists(filePath.c_str());}
		static bool DirectoryExists(const char * filePath);
	private:
		bool					m_bInited;
		std::vector<std::string> m_paths;
	};
}
#endif //_FILESYSTEM_H_