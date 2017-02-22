
#include "pch.h"
#include "XMLWarpper.h"

namespace Aurora
{
	tinyxml2::XMLElement * XMLElementWrapper::GetChildRaw( const std::string & strChildPath )const
	{
		if(!m_pElement)
			return 0;
		std::string::size_type pos = strChildPath.find_first_of("/\\");
		if(pos == strChildPath.npos)
		{
			return m_pElement->FirstChildElement(strChildPath.c_str());
		}
		tinyxml2::XMLElement * pEle = m_pElement->FirstChildElement(strChildPath.substr(0,pos).c_str());
		if(!pEle)
			return 0;
		return XMLElementWrapper(pEle).GetChildRaw(strChildPath.substr(pos+1));
	}


	tinyxml2::XMLElement * XMLElementWrapper::GetNextSiblingRaw( const std::string & strChildPath ) const
	{
		if(!m_pElement)
			return 0;
		return m_pElement->NextSiblingElement(strChildPath.c_str());
	}

	bool XMLElementWrapper::GetAttribute( const char * strAttri,std::string & attrValue,const std::string & defaultValue/*=std::string()*/ )const
	{
		if(!m_pElement || !strAttri)
		{
			attrValue = defaultValue;
			return false;
		}

		const char * pAttr = m_pElement->Attribute(strAttri);
		if(!pAttr)
		{
			attrValue = defaultValue;
			return false;
		}

		attrValue.assign(pAttr);
		return true;
	}

	bool XMLElementWrapper::GetAttribute( const char * strAttri,bool & attrValue,bool defaultValue /*= bool()*/ )const
	{
		if(!m_pElement || !strAttri || tinyxml2::XML_NO_ATTRIBUTE == m_pElement->QueryBoolAttribute(strAttri,&attrValue))
		{
			attrValue = defaultValue;
			return false;
		}
		return true;
	}

	bool XMLElementWrapper::GetAttribute( const char * strAttri,int8 & attrValue,int8 defaultValue /*= int8()*/ )const
	{
		int val;
		if(!m_pElement|| !strAttri  || tinyxml2::XML_NO_ATTRIBUTE == m_pElement->QueryIntAttribute(strAttri,&val))
		{
			attrValue = defaultValue;
			return false;
		}
		attrValue = int8(val);
		return true;
	}

	bool XMLElementWrapper::GetAttribute( const char * strAttri,int16 & attrValue,int16 defaultValue /*= int16()*/ )const
	{
		MY_ASSERT(m_pElement);
		int val;
		if(!m_pElement || !strAttri || tinyxml2::XML_NO_ATTRIBUTE == m_pElement->QueryIntAttribute(strAttri,&val))
		{
			attrValue = defaultValue;
			return false;
		}
		attrValue = val;
		return true;
	}

	bool XMLElementWrapper::GetAttribute( const char * strAttri,int32 & attrValue,int32 defaultValue /*= int32()*/ )const
	{
		MY_ASSERT(m_pElement);
		int val;
		if(!m_pElement || !strAttri || tinyxml2::XML_NO_ATTRIBUTE == m_pElement->QueryIntAttribute(strAttri,&val))
		{
			attrValue = defaultValue;
			return false;
		}
		attrValue = val;
		return true;
	}

	bool XMLElementWrapper::GetAttribute(  const char * strAttri ,int64 & attrValue,int64 defaultValue /*= int64()*/ )const
	{
		MY_ASSERT(m_pElement);
		if(!m_pElement || !strAttri)
		{
			attrValue = defaultValue;
			return false;
		}

		const char * pAttr = m_pElement->Attribute(strAttri);
		if(!pAttr)
		{
			attrValue = defaultValue;
			return false;
		}
#ifdef WIN32
		attrValue = _atoi64(pAttr);
#else
		attrValue = atoll(pAttr);
#endif
		return true;
	}

	bool XMLElementWrapper::GetAttribute( const char * strAttri,uint8 & attrValue, uint8 defaultValue /*= uint8()*/ )const
	{
		MY_ASSERT(m_pElement);
		unsigned val;
		if(!m_pElement|| !strAttri  || tinyxml2::XML_NO_ATTRIBUTE == m_pElement->QueryUnsignedAttribute(strAttri,&val))
		{
			attrValue = defaultValue;
			return false;
		}
		attrValue = uint8(val);
		return true;
	}

	bool XMLElementWrapper::GetAttribute( const char * strAttri,uint16 & attrValue,uint16 defaultValue /*= uint16()*/ )const
	{
		MY_ASSERT(m_pElement);
		unsigned val;
		if(!m_pElement|| !strAttri  || tinyxml2::XML_NO_ATTRIBUTE == m_pElement->QueryUnsignedAttribute(strAttri,&val))
		{
			attrValue = defaultValue;
			return false;
		}
		attrValue = uint16(val);
		return true;
	}

	bool XMLElementWrapper::GetAttribute( const char * strAttri,uint32 & attrValue,uint32 defaultValue /*= uint32()*/ )const
	{
		MY_ASSERT(m_pElement);
		unsigned val;
		if(!m_pElement|| !strAttri  || tinyxml2::XML_NO_ATTRIBUTE == m_pElement->QueryUnsignedAttribute(strAttri,&val))
		{
			attrValue = defaultValue;
			return false;
		}
		attrValue = uint32(val);
		return true;
	}

	bool XMLElementWrapper::GetAttribute(  const char * strAttri,uint64 & attrValue,uint64 defaultValue /*= uint64()*/ )const
	{
		MY_ASSERT(m_pElement);
		if(!m_pElement || !strAttri)
		{
			attrValue = defaultValue;
			return false;
		}

		const char * pAttr = m_pElement->Attribute(strAttri);
		if(!pAttr)
		{
			attrValue = defaultValue;
			return false;
		}

#ifdef WIN32
		attrValue = _strtoui64(pAttr,0,10);
#else
		attrValue = strtoul(pAttr,0,10);
#endif
		return true;
	}

	bool XMLElementWrapper::GetAttribute( const char * strAttri,f32 & attrValue,f32 defaultValue /*= f32()*/ )const
	{
		MY_ASSERT(m_pElement);
		if(!m_pElement || !strAttri || tinyxml2::XML_NO_ATTRIBUTE == m_pElement->QueryFloatAttribute(strAttri,&attrValue))
		{
			attrValue = defaultValue;
			return false;
		}
		return true;
	}

	bool XMLElementWrapper::GetAttribute( const char * strAttri,f64 & attrValue,f64 defaultValue /*= f64()*/ )const
	{
		MY_ASSERT(m_pElement);
		if(!m_pElement || !strAttri || tinyxml2::XML_NO_ATTRIBUTE == m_pElement->QueryDoubleAttribute(strAttri,&attrValue))
		{
			attrValue = defaultValue;
			return false;
		}
		return true;
	}
}

