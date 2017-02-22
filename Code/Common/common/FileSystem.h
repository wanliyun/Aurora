#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include <string>
#include <vector>
#include <boost/serialization/singleton.hpp>

namespace Aurora
{
	//����ģʽ;
	class  FileSystem : public boost::serialization::singleton<FileSystem>
	{
	public:
		FileSystem():m_bInited(false){}
		~FileSystem(){}

		/** ���ļ���ʼ���ļ�ϵͳ;
		 *	@param szInitFilePath �ļ������exe�ļ������·�����߾���·��;
		 *	@return �Ƿ�ɹ�;
		 *	@note 
		 */
		bool Init(const char * szInitFilePath);

		/** �����·�����������·����;
		 *	@param szPath ���·��;
		 *	@return �ļ��ľ���·��  ����0��ʾû���ҵ��ļ�;
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