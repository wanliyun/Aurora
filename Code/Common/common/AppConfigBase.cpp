
#include "pch.h"
#include "AppConfigBase.h"
#include <tinyxml2.h>
#include <FileSystem.h>
#include "XMLWarpper.h"

namespace Aurora
{
	AppConfigBase::AppConfigBase(const std::string & strFile,const std::string & strPrefix):
		m_strFile(strFile),
		m_strPrefix(strPrefix)
	{
		
	}

	bool AppConfigBase::Init()
	{
		tinyxml2::XMLDocument doc;
		std::string absFilePath = FileSystem::get_mutable_instance().FindFilePath(m_strFile);
		if(tinyxml2::XML_NO_ERROR != doc.LoadFile(absFilePath.c_str()))
		{
			Log_ErrorF("load file %s",absFilePath.c_str());
			return false;
		}

		tinyxml2::XMLHandle hDoc(&doc);
		XMLElementWrapper xmlWrapper(hDoc.FirstChildElement().ToElement());
		if(!xmlWrapper)
			return false;
		return this->OnInit(xmlWrapper.GetChild(m_strPrefix));
	}

}
