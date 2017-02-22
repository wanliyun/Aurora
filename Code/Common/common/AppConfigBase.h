#ifndef _APP_CONFIG_BASE_H_
#define _APP_CONFIG_BASE_H_

#include "interface/INoneCopyable.h"
#include <string>

namespace Aurora
{
	class XMLElementWrapper;
	class AppConfigBase : public INoneCopyable
	{
	public:
		AppConfigBase(const std::string & strFile,const std::string & strPrefix);
		bool Init();
	protected:
		virtual bool OnInit(const XMLElementWrapper & xmlEle) = 0;
	private:
		const std::string m_strFile;
		const std::string m_strPrefix;
	};
#define PREPARE_CONFIG()\
	if( !xmlEle)\
		return false;\
	bool bSucc = true;\
	XMLElementWrapper ele(xmlEle.GetPtr())


#define BEGIN_CONFIG(CHILD_NAME)\
	do{\
		tinyxml2::XMLElement * const ____pBackUp = ele.GetPtr();\
		if(!____pBackUp)\
		{\
			bSucc = false;\
			break;\
		}\
		ele = ele.GetChild(#CHILD_NAME);\
		if(!ele)\
		{\
			bSucc = false;\
			break;\
		}\
		(void)(0)
#define BEGIN_CONFIG_LIST(CHILD_NAME)\
	do\
	{\
		tinyxml2::XMLElement * const ____pBackUp =ele.GetPtr();\
		if(!____pBackUp)\
		{\
			bSucc = false;\
			break;\
		}\
		do{\
			for(ele = ele.GetChild(#CHILD_NAME);ele;ele = ele.GetNextSibling(#CHILD_NAME))\
			{(void)(0)

#define CONFIG_ATTRIBUTE(NAME,RESULT)\
		bSucc &= ele.GetAttribute(#NAME,RESULT)

#define END_CONFIG()\
		ele.SetPtr(____pBackUp);\
	}while(false)

#define END_CONFIG_LIST()\
			}\
		}while(false);\
		ele.SetPtr(____pBackUp);\
	} while (false)


#define CONFIG_RETURN()\
	return bSucc
}

#endif
