#ifndef _XML_WRAPPER_H_
#define _XML_WRAPPER_H_

#include <tinyxml2.h>
#include <string.h>
#include <Types.h>
namespace Aurora
{
	class  XMLElementWrapper
	{
	public:
		explicit XMLElementWrapper(tinyxml2::XMLElement * pEle = 0):m_pElement(pEle)
		{

		}
		~XMLElementWrapper(){}

		operator bool()const{return m_pElement != 0;}

		void SetPtr(tinyxml2::XMLElement * pEle){m_pElement = pEle;}
		tinyxml2::XMLElement * GetPtr()const{return m_pElement;}

		inline XMLElementWrapper GetChild(const std::string & strChildPath)const{return XMLElementWrapper(GetChildRaw(strChildPath));}
		tinyxml2::XMLElement * GetChildRaw(const std::string & strChildPath)const;

		inline XMLElementWrapper GetNextSibling(const std::string & strName)const{return XMLElementWrapper(GetNextSiblingRaw(strName));}
		tinyxml2::XMLElement * GetNextSiblingRaw(const std::string & strName)const;

		bool GetAttribute(const char * strAttri,std::string & attrValue,const std::string & defaultValue=std::string())const;								  
		bool GetAttribute(const char * strAttri,bool & attrValue,bool defaultValue = bool())const;	
		bool GetAttribute(const char * strAttri,int8 & attrValue,int8 defaultValue = int8())const;
		bool GetAttribute(const char * strAttri,int16 & attrValue,int16 defaultValue = int16())const;
		bool GetAttribute(const char * strAttri,int32 & attrValue,int32 defaultValue = int32())const;
		bool GetAttribute(const char * strAttri,int64 & attrValue,int64 defaultValue = int64())const;		  
		bool GetAttribute(const char * strAttri,uint8 & attrValue, uint8 defaultValue =  uint8())const;
		bool GetAttribute(const char * strAttri,uint16 & attrValue,uint16 defaultValue = uint16())const;
		bool GetAttribute(const char * strAttri,uint32 & attrValue,uint32 defaultValue = uint32())const;
		bool GetAttribute(const char * strAttri,uint64 & attrValue,uint64 defaultValue = uint64())const; 
		bool GetAttribute(const char * strAttri,f32 & attrValue,f32 defaultValue = f32())const;
		bool GetAttribute(const char * strAttri,f64 & attrValue,f64 defaultValue = f64())const;

		template<typename ValueT>
		inline bool GetAttribute(const std::string & strAttri,ValueT & attrValue,ValueT defaultValue = ValueT())const
		{
			return GetAttribute(strAttri.c_str(),attrValue);
		}


	private:
		tinyxml2::XMLElement * m_pElement;		//²»ÓµÓÐ;
	};
}
#endif //_XML_WRAPPER_H_