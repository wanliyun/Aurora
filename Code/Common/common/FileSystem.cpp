
#include "pch.h"
#include "FileSystem.h"
#include <fstream>
#include <iostream>
#include <string>
#include <tinyxml2.h>

namespace Aurora
{

	bool FileSystem::Init( const char * szPath )
	{
		assert(!m_bInited);
		assert(szPath);
		if(!szPath)
			return false;

		tinyxml2::XMLDocument doc;
		if(tinyxml2::XML_NO_ERROR != doc.LoadFile(szPath))
			return false;

		tinyxml2::XMLElement * ele = 0;
		ele = doc.FirstChildElement("Paths");
		if(!ele)
			return false;
		ele = ele->FirstChildElement("Path");
		while(ele)
		{
			if(ele->GetText())
			{
				std::string  path = std::string(ele->GetText()) ;
				if(!path.empty())
				{
					m_paths.push_back(path );
				}
			}
			ele = ele->NextSiblingElement();
		}
		
		m_bInited = m_paths.size() != 0;
		return m_bInited;
	}

	std::string FileSystem::FindFilePath( const char * szPath ) const
	{
		assert(szPath);
		if(!szPath)
			return 0;
		std::string strPath;
		for(std::vector<std::string>::const_iterator it = m_paths.begin();it!=m_paths.end();++it)
		{
			strPath = *it + szPath;
			if(FileExists(strPath.c_str()))
				return strPath;
		}
		return std::string();
	}

	std::string FileSystem::FindDirectoryPath( const char * szPath ) const
	{
		assert(szPath);
		if(!szPath)
			return 0;
		std::string strPath;
		for(std::vector<std::string>::const_iterator it = m_paths.begin();it!=m_paths.end();++it)
		{
			strPath = *it + szPath;
			if(DirectoryExists(strPath.c_str()))
				return strPath;
		}
		return std::string();
	}


	bool FileSystem::FileExists( const char * filePath )
	{
		assert(filePath);
		if(!filePath)
			return 0;
#ifdef WIN32
		WIN32_FIND_DATA FindFileData;
		HANDLE handle = FindFirstFile(filePath, &FindFileData) ;
		bool bFound = handle != INVALID_HANDLE_VALUE;
		if(bFound) 
			FindClose(handle);
		return bFound && (  (FindFileData.dwFileAttributes &(FILE_ATTRIBUTE_NORMAL|FILE_ATTRIBUTE_ARCHIVE ) )  != 0);
#else
		struct stat buf;
		if(0!=stat(filePath, &buf))
			return false;
		return (buf.st_mode & S_IFREG)  !=0; 
#endif
	}

	bool FileSystem::DirectoryExists( const char * filePath )
	{
		assert(filePath);
		if(!filePath)
			return 0;
#ifdef WIN32
		WIN32_FIND_DATA FindFileData;
		HANDLE handle = FindFirstFile(filePath, &FindFileData) ;
		bool bFound = handle != INVALID_HANDLE_VALUE;
		if(bFound) 
			FindClose(handle);
		return bFound &&  (  (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  != 0);
#else
		struct stat buf;
		if(0!=stat(filePath, &buf))
			return false;
		return (buf.st_mode & S_IFDIR )  !=0; 
#endif
	}
}

