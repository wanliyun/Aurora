#ifndef _DATA_TYPE_INTERGER_H
#define _DATA_TYPE_INTERGER_H

#include "DataTypeBase.h"
#include "binary_stream.h"
#include <boost/lexical_cast.hpp>
#include "PyTypeLongInteger.h"
#include "MetaDataTypeBase.h"

namespace Aurora
{
	class DataTypeBool : public DataTypeBase
	{
	public:
		DataTypeBool(MetaDataTypeBase * pMetaType):DataTypeBase(pMetaType){m_strDftValue = "0";}

		virtual bpy::object NewObj(AttributeFlagT /*flag*/)const;
		virtual bpy::object NewObj(BinaryIStream & iStream)const;
		virtual bool WriteToStream(BinaryOStream & oStream,bpy::object & obj,AttributeFlagT /*flag*/)const;
	};

	template<typename TYPE>
	class DataTypeInt : public DataTypeBase
	{
	public:
		DataTypeInt(MetaDataTypeBase * pMetaType):DataTypeBase(pMetaType){m_strDftValue = "0";}
		
		virtual bpy::object NewObj(AttributeFlagT /*flag*/)const
		{
			return bpy::long_( boost::lexical_cast<TYPE>(m_strDftValue) );
		}
		
		virtual bpy::object NewObj(BinaryIStream & iStream)const
		{
			TYPE val;
			iStream>>val;
			return bpy::long_(val);
		}

		virtual bool WriteToStream(BinaryOStream & oStream,bpy::object & obj,AttributeFlagT /*flag*/)const
		{
			try
			{
				TYPE val = bpy::extract<TYPE>(obj);
				oStream<<TYPE(val);
			}
			catch(bpy::error_already_set)
			{
				PyErr_Print();
				return false;
			}

			return true;
		}
	};

	template<>
	bpy::object DataTypeInt<int8>::NewObj(AttributeFlagT /*flag*/)const
	{
		int32 val = boost::lexical_cast<int32>(m_strDftValue);
		return bpy::long_( int32(int8(val)) );
	}

	template<>
	bpy::object DataTypeInt<uint8>::NewObj(AttributeFlagT /*flag*/)const
	{
		uint32 val = boost::lexical_cast<int32>(m_strDftValue);
		return bpy::long_( uint32(uint8(val)) );
	}

	template<typename TYPE>
	class DataTypeFloat : public DataTypeBase
	{
	public:
		DataTypeFloat(MetaDataTypeBase * pMetaType):DataTypeBase(pMetaType){m_strDftValue = "0";}

		virtual bpy::object NewObj(AttributeFlagT /*flag*/)const
		{
			return bpy::object(boost::lexical_cast<TYPE>(m_strDftValue));
		}

		virtual bpy::object NewObj(BinaryIStream & iStream)const
		{
			TYPE val;
			iStream>>val;
			return bpy::object(val);
		}

		virtual bool WriteToStream(BinaryOStream & oStream,bpy::object & obj,AttributeFlagT /*flag*/)const
		{
			try
			{
				TYPE val = bpy::extract<TYPE>(obj);
				oStream<<TYPE(val);
				return true;
			}
			catch(bpy::error_already_set)
			{
				PyErr_Print();
			}

			return false;
		}
	private:
	};

	class DataTypeString : public DataTypeBase
	{
	public:
		DataTypeString(MetaDataTypeBase * pMetaType):DataTypeBase(pMetaType){m_strDftValue="";}

		virtual bpy::object NewObj(AttributeFlagT /*flag*/)const
		{
			return bpy::str(m_strDftValue);
		}

		virtual bpy::object NewObj(BinaryIStream & iStream)const
		{
			std::string val;
			iStream>>val;
			return bpy::str(val);
		}
		virtual bool WriteToStream(BinaryOStream & oStream,bpy::object & obj,AttributeFlagT )const
		{
			const char * pStr = bpy::extract<const char * >(obj);
			oStream<<pStr;
			return true;
		}
	};

	template<typename DataTypeT>
	class MetaDataTypeSimple : public MetaDataTypeBase
	{
	public:
		MetaDataTypeSimple(const std::string & strTypeName):MetaDataTypeBase(strTypeName)
		{

		}

		virtual DataTypeBase * NewType(tinyxml2::XMLHandle & xml)
		{
			tinyxml2::XMLElement * pEle = xml.ToElement();
			if(!pEle)
			{
				Log_ErrorF("xml is NULL\n");
				return 0;
			}

			DataTypeT * pTypeRet = new DataTypeT(this);

			const char * pSzFlag = pEle->Attribute("flag");
			if(pSzFlag)
			{
				pTypeRet->SetFlag(pSzFlag);
			}
			
			const char * pSzDftValue = pEle->Attribute("default");
			if(pSzDftValue)
			{
				pTypeRet->SetDftValue(pSzDftValue);
			}
			return pTypeRet;
		}
	};
}

#endif
