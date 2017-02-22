#include "pch.h"
#include "AttributeOwnerBase.h"
#include "DataTypeSimple.h"
#include "DataTypeFixedDict.h"

namespace Aurora
{
	std::vector<MetaDataTypeBase*> MetaDataTypeBase::m_vMetaTypes;
	std::map<std::string,MetaDataTypeBase*> MetaDataTypeBase::s_mapName2MetaTypes;

	void MetaDataTypeBase::AddMetaType( MetaDataTypeBase * type )
	{
		MY_ASSERT(type);
		bool bSucc = s_mapName2MetaTypes.insert(std::make_pair(type->GetTypeName(),type)).second;
		MY_ASSERT(bSucc);
	}


	void MetaDataTypeBase::AddAlias( const std::string & strType,const std::string & strOriType )
	{
		MetaDataTypeBase * pType = FindMetaType(strOriType);
		if(!pType)
		{
			Log_ErrorF("[%s] is not a meta type\n",strOriType.c_str());
			return;
		}

		if(! s_mapName2MetaTypes.insert(std::make_pair(strType,pType)).second )
		{
			Log_ErrorF("[%s] is already a metatype\n",strType.c_str());
			return;
		}
	}


	bool MetaDataTypeBase::Init(tinyxml2::XMLHandle & xml)
	{
		MY_ASSERT(s_mapName2MetaTypes.empty());

		//////////////////////////////////////////////////////////////////////////
		//系统;
		m_vMetaTypes.push_back( new MetaDataTypeSimple< DataTypeBool >("bool") );

		m_vMetaTypes.push_back( new MetaDataTypeSimple< DataTypeInt<int8> > ("int8") );
		m_vMetaTypes.push_back( new MetaDataTypeSimple< DataTypeInt<int16> >("int16") );
		m_vMetaTypes.push_back( new MetaDataTypeSimple< DataTypeInt<int32> >("int32") );
		m_vMetaTypes.push_back( new MetaDataTypeSimple< DataTypeInt<int64> >("int64") );

		m_vMetaTypes.push_back( new MetaDataTypeSimple< DataTypeInt<uint8> > ("uint8") );
		m_vMetaTypes.push_back( new MetaDataTypeSimple< DataTypeInt<uint16> >("uint16") );
		m_vMetaTypes.push_back( new MetaDataTypeSimple< DataTypeInt<uint32> >("uint32") );
		m_vMetaTypes.push_back( new MetaDataTypeSimple< DataTypeInt<uint64> >("uint64") );


		m_vMetaTypes.push_back( new MetaDataTypeSimple< DataTypeFloat<f32> >("f32") );
		m_vMetaTypes.push_back( new MetaDataTypeSimple< DataTypeFloat<f64> >("f64") );

		m_vMetaTypes.push_back( new MetaDataTypeSimple< DataTypeString >("string") );

		m_vMetaTypes.push_back( new DataTypeFixedDictMeta("FIXED_DICT") );

		for( std::vector<MetaDataTypeBase*>::iterator it = m_vMetaTypes.begin();it!=m_vMetaTypes.end();++it )
		{
			AddMetaType(*it);
		}

		//////////////////////////////////////////////////////////////////////////
		//系统别名;
		AddAlias("BOOL",		"bool");

		AddAlias("INT8",		"int8");
		AddAlias("INT16",		"int16");
		AddAlias("INT32",		"int32");
		AddAlias("INT64",		"int64");

		AddAlias("UINT8",		"uint8");
		AddAlias("UINT16",		"uint16");
		AddAlias("UINT32",		"uint32");
		AddAlias("UINT64",		"uint64");

		AddAlias("F32",			"f32");
		AddAlias("F64",			"f64");

		AddAlias("FLOAT",		"f32");
		AddAlias("FLOAT32",		"f32");
		AddAlias("FLOAT64",		"f64");

		AddAlias("STRING",		"string");

		//////////////////////////////////////////////////////////////////////////
		//用户自定义别名;
		for( tinyxml2::XMLElement * pEnt  = xml.FirstChildElement().ToElement(); pEnt; pEnt=pEnt->NextSiblingElement() )
		{
			std::string strName(pEnt->Name());

			const char * pSzMetaType = pEnt->GetText();
			if(!pSzMetaType)
			{
				Log_ErrorF("[%s] No metaType Specified\n",strName.c_str());
				continue;
			}

			AddAlias(strName,pSzMetaType);
		}
		return true;
	}

	void MetaDataTypeBase::Fini()
	{
		s_mapName2MetaTypes.clear();

		for( std::vector<MetaDataTypeBase*>::iterator it = m_vMetaTypes.begin();it!=m_vMetaTypes.end();++it )
		{
			SAFE_DELETE(*it);
		}
		m_vMetaTypes.clear();
	}

	MetaDataTypeBase * MetaDataTypeBase::FindMetaType( const std::string & strType )
	{
		std::map<std::string,MetaDataTypeBase*>::iterator iter = s_mapName2MetaTypes.find(strType);
		if(iter ==s_mapName2MetaTypes.end())
			return 0;
		return iter->second;
	}

}

