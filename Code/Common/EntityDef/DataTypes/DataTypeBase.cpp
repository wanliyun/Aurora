#include "pch.h"
#include "DataTypeBase.h"
#include "MetaDataTypeBase.h"
#include "FileSystem.h"

namespace Aurora
{
	DataTypeBase::MapTypesT DataTypeBase::s_mapTypes;

	DataTypeBase::DataTypeBase( MetaDataTypeBase * pMetaType )
		:m_pMetaType(pMetaType)
		,m_nRefIndex(BAD_ATTRIBUTE_ID)
		,m_flag(0)
	{
	}

	void DataTypeBase::AddToMD5( MD5 & md5 ) const
	{
		//md5.Append(m_name.c_str(),m_name.size());
	}

	bool DataTypeBase::Init( const std::string & strAliasTypeFile )
	{
		tinyxml2::XMLDocument doc;

		std::string absFilePath = FileSystem::get_mutable_instance().FindFilePath(strAliasTypeFile);
		if(absFilePath.empty() || tinyxml2::XML_NO_ERROR != doc.LoadFile(absFilePath.c_str()))
		{
			Log_CriticalF("can not find file[%s]\n",strAliasTypeFile.c_str());
			return false;
		}

		tinyxml2::XMLHandle hDoc(&doc);

		MetaDataTypeBase::Init(hDoc.FirstChildElement("root").FirstChildElement("alias"));

		tinyxml2::XMLElement * pEnt  = hDoc.FirstChildElement("root").FirstChildElement("typeDefine").FirstChildElement().ToElement();
		for( ; pEnt; pEnt=pEnt->NextSiblingElement() )
		{
			const std::string strNewTypeName = pEnt->Name();
			if(MetaDataTypeBase::FindMetaType(strNewTypeName) )
			{
				Log_ErrorF("new type[%s] is a meta type",strNewTypeName.c_str());
				continue;
			}

			if(GetType(strNewTypeName))
			{
				Log_ErrorF("new type[%s] is already defined",strNewTypeName.c_str());
				continue;
			}

			const char * pSzMetaType = pEnt->Attribute("type");
			if(!pSzMetaType)
			{
				Log_ErrorF("Bad DataType [%s] No metaType Specified",strNewTypeName.c_str());
				continue;
			}
			
			MetaDataTypeBase * pMetaType = MetaDataTypeBase::FindMetaType(pSzMetaType);
			if(!pMetaType)
			{
				Log_ErrorF("Bad DataType for [%s]  base [%s]",strNewTypeName.c_str(),pEnt->GetText());
				continue;
			}

			boost::shared_ptr<DataTypeBase> pNewDataType( pMetaType->NewType(tinyxml2::XMLHandle(pEnt)) );
			if(!pNewDataType)
			{
				Log_ErrorF("Build DataType Faield [%s] [%s]",strNewTypeName.c_str(),pEnt->GetText());
				continue;
			}

			s_mapTypes.insert(std::make_pair(strNewTypeName,pNewDataType));
		}
		return true;
	}

	void DataTypeBase::Fini()
	{
	}

	boost::shared_ptr<DataTypeBase> DataTypeBase::GetType( const std::string & strTypeName )
	{
		MapTypesT::const_iterator iterType = s_mapTypes.find(strTypeName);
		if(iterType != s_mapTypes.end())
			return iterType->second;

		return 0;
	}

	DataTypeBasePtr DataTypeBase::CreateType( const std::string & strTypeName,tinyxml2::XMLHandle xml )
	{
		MetaDataTypeBase * pMetaType = MetaDataTypeBase::FindMetaType(strTypeName);
		if(!pMetaType)
			return 0;
		return boost::shared_ptr<DataTypeBase>(pMetaType->NewType(xml));
	}

	bool DataTypeBase::SetFlag( const std::string & strFlag )
	{
		std::string::size_type idx = 0;
		std::string::size_type len = strFlag.size();
		AttributeFlagT oriFlag = m_flag;
		m_flag = 0;
		uint32 nFlags = 0; 

		for(;idx < len && nFlags < MAX_FLAGS;++idx)
		{
			char ch = strFlag[idx];
			if(ch != '1' && ch != '0')
				continue;
			int nVal = int(ch -'0');
			m_flag = (m_flag << 1)  | nVal;
			++nFlags;
		}
		if(nFlags != MAX_FLAGS)
		{
			m_flag = oriFlag;
			return false;
		}

		if(HasAnyFlag(EFlag_OwnbyCell) && !HasAnyFlag(EFlag_RealCell))
		{
			Log_ScriptF("Flag EFlag_RealCell is turned on when has flag EFlag_OwnbyCell\n");
			SetFlag(EFlag_RealCell);
		}
		return true;
	}
}
