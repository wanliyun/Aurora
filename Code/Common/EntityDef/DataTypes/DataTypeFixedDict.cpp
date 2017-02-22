#include "pch.h"
#include "DataTypeFixedDict.h"
#include "MetaDataTypeBase.h"

namespace Aurora
{
	DataTypeBase * DataTypeFixedDictMeta::NewType( tinyxml2::XMLHandle & xml )
	{
		std::auto_ptr<DataTypeFixDict>  pRetType ( new DataTypeFixDict(this) );
		if(BuildDictType(*pRetType.get(),xml))
			return pRetType.release();
		return 0;
	}

	bool DataTypeFixedDictMeta::BuildDictType( DataTypeFixDict & dictType ,tinyxml2::XMLHandle & xml)
	{
		tinyxml2::XMLElement * pElement = xml.FirstChildElement().ToElement();
		for(;pElement;pElement = pElement->NextSiblingElement())
		{
			const char * pSzAttrName = pElement->Name();
			if(dictType.GetChildTypeByName(pSzAttrName))
			{
				Log_ErrorF("Can not fine attribute [%s] already exists\n",pSzAttrName);
				return false;
			}

			const char * pSzTypeName = pElement->Attribute("type");
			if(!pSzTypeName)
			{
				Log_ErrorF("Can not fine attribute [type]\n");
				return false;
			}
			DataTypeBasePtr  pChildType = DataTypeBase::GetType(pSzTypeName);
			
			if(!pChildType)
			{
				pChildType = DataTypeBase::CreateType(pSzTypeName,tinyxml2::XMLHandle(pElement));
				if(pChildType)
				{
					const char * pSzFlag = pElement->Attribute("flags");
					if(pSzFlag)
					{
						pChildType->SetFlag(pSzFlag);
					}
				}
				else
				{
					Log_ErrorF("attrbute [%s] has bad attribute[%s]\n",pSzAttrName,pSzTypeName);
					return false;
				}
				dictType.AppendChildType(pSzAttrName,pChildType);
			}
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	bpy::object DataTypeFixDict::NewObj(AttributeFlagT flag) const
	{
		DataTypeFixDict_PyObjectPtr ptr(new DataTypeFixDict_PyObject(this));
		ptr->Create(flag);
		return bpy::object(ptr);
	}

	bpy::object DataTypeFixDict::NewObj( BinaryIStream & iStream ) const
	{
		DataTypeFixDict_PyObjectPtr ptr(new DataTypeFixDict_PyObject(this));
		ptr->CreateFromStream(iStream);
		return bpy::object(ptr);
	}

	bool DataTypeFixDict::WriteToStream( BinaryOStream & oStream,bpy::object & obj,AttributeFlagT flag ) const
	{
		DataTypeFixDict_PyObject & dict = bpy::extract<DataTypeFixDict_PyObject&>(obj);
		return dict.WriteToStream(oStream,obj,flag);
	}

	AttributeOwnerBase * DataTypeFixDict::AsAttributeOwner( bpy::object obj )
	{
		DataTypeFixDict_PyObject & dict = bpy::extract<DataTypeFixDict_PyObject & >(obj);
		return &dict;
	}

	void DataTypeFixDict::OnAttach( bpy::object obj,AttributeOwnerBase * pOwner,uint32 indexInOwner )
	{
		MY_ASSERT(pOwner);

		DataTypeFixDict_PyObject & dict = bpy::extract<DataTypeFixDict_PyObject & >(obj);
		dict.SetOwner(pOwner,indexInOwner);
	}

	void DataTypeFixDict::OnDetach( bpy::object obj )
	{
		DataTypeFixDict_PyObject & dict = bpy::extract<DataTypeFixDict_PyObject & >(obj);
		dict.SetOwner(0,BAD_ATTRIBUTE_ID);
	}

	void DataTypeFixDict::AppendChildType( const std::string & strName,boost::shared_ptr<DataTypeBase> pChildType )
	{
		MY_ASSERT(pChildType);
		if(GetChildTypeByName(strName))
		{
			Log_ErrorF("Can not add attribute [%s] already exists\n",strName.c_str());
			return;
		}

		m_vChilds.push_back(pChildType);
		AttributeID idx = AttributeID(m_vChilds.size() -1 );
		pChildType->SetRefName(strName);
		pChildType->SetRefId(idx);

		m_mapName2ID.insert(std::make_pair(strName,idx));
		m_mapID2Name.insert(std::make_pair(idx,strName));
	}

	boost::shared_ptr<DataTypeBase> DataTypeFixDict::GetChildTypeByID( AttributeID nIndex )const
	{
		if(nIndex >= GetChildCount())
			return 0;
		return m_vChilds[nIndex];
	}

	boost::shared_ptr<DataTypeBase> DataTypeFixDict::GetChildTypeByName( const std::string & strName )const
	{
		Name2IDMapT::const_iterator iter = m_mapName2ID.find(strName);
		if(iter == m_mapName2ID.end())
			return 0;
		return GetChildTypeByID(iter->second);
	}

	bool DataTypeFixDict::GetChildIdByName( const std::string & strName, AttributeID & id )const
	{
		Name2IDMapT::const_iterator iter = m_mapName2ID.find(strName);
		if(iter == m_mapName2ID.end())
			return false;
		id = iter->second;
		return true;
	}

	bool DataTypeFixDict::GetChildNameByID(AttributeID nIndex,std::string & strName)const
	{
		ID2NameMapT::const_iterator iter = m_mapID2Name.find(nIndex);
		if(iter == m_mapID2Name.end())
			return false;
		strName = iter->second;
		return true;
	}

	void DataTypeFixDict::Inhert( DataTypeFixDict & childType )
	{
		std::string strName;
		for(AttributeID idx = 0;idx < GetChildCount();++idx)
		{
			bool bRet = GetChildNameByID(idx,strName);
			MY_ASSERT(bRet);
			childType.AppendChildType(strName,m_vChilds[idx]);
		}
	}



	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DataTypeFixDict_PyObject::DataTypeFixDict_PyObject(const DataTypeFixDict * pDataType )
		:m_dataType( *pDataType )
	{
	}

	bool DataTypeFixDict_PyObject::Create( AttributeFlagT flag )
	{
		for(AttributeID i = 0;i<m_dataType.GetChildCount();++i)
		{
			DataTypeBasePtr childType =m_dataType.GetChildTypeByID(i);
			if(!childType->HasAnyFlag(flag))
				continue;
			bpy::object obj = childType->NewObj(flag);
			m_mapName2ChildObjs.insert(std::make_pair(childType->GetRefName(),obj));
			childType->OnAttach(obj,this,i);
		}
		return true;
	}

	bool DataTypeFixDict_PyObject::CreateFromStream( BinaryIStream & iStream )
	{
		uint16 nAttriButeCount = 0;
		iStream>>nAttriButeCount;

		AttributeID id;
		for(uint16 nAttr = 0;nAttr < nAttriButeCount;++nAttr)
		{
			iStream>>id;
			if(id > m_dataType.GetChildCount())
			{
				Log_ErrorF("Bad index=[%u]",id);
				return false;
			}

			DataTypeBasePtr childType = m_dataType.GetChildTypeByID(id);

			bpy::object obj = childType->NewObj(iStream);
			m_mapName2ChildObjs.insert(std::make_pair(childType->GetRefName(),obj));
			childType->OnAttach(obj,this,id);
		}

		return !iStream.error();
	}

	bool DataTypeFixDict_PyObject::HasKey(const std::string & strKey)const
	{
		return !m_dataType.GetChildTypeByName(strKey);
	}

	bpy::object DataTypeFixDict_PyObject::GetKeys()
	{
		bpy::list listRet;
		for(AttributeID i = 0;i<m_dataType.GetChildCount();++i)
		{
			boost::shared_ptr<DataTypeBase> pChildType = m_dataType.GetChildTypeByID(i);
			
			if(m_mapName2ChildObjs.find(pChildType->GetRefName()) == m_mapName2ChildObjs.end())
				continue;

			listRet.append( pChildType->GetRefName() );
		}
		return listRet;
	}

	bpy::object DataTypeFixDict_PyObject::GetValues()
	{
		bpy::list listRet;
		for(MapName2ChildObjT::iterator iter = m_mapName2ChildObjs.begin();iter != m_mapName2ChildObjs.end();++iter)
		{
			listRet.append( iter->second );
		}
		return listRet;
	}

	bpy::object DataTypeFixDict_PyObject::GetItems()
	{
		return bpy::object();
	}

	bool DataTypeFixDict_PyObject::OnPreSetAttr( bpy::object obj,const std::string & attr, bpy::object val )
	{
		DataTypeFixDict_PyObject & dict = bpy::extract<DataTypeFixDict_PyObject &>(obj);

		boost::shared_ptr<DataTypeBase> pChildType = dict.m_dataType.GetChildTypeByName(attr);
		if(!pChildType)
			return false;
		
		MapName2ChildObjT::iterator iter = m_mapName2ChildObjs.find(pChildType->GetRefName());
		if(iter == m_mapName2ChildObjs.end())
			return false;

		if(!GetOwner())
			return false;

		SingleAttributeChange chg;
		chg.m_index = pChildType->GetRefId();
		chg.m_newVal = iter->second;
		chg.m_vPath.push_back(m_dataType.GetRefId());
		GetOwner()->OnSingleAttributeChange(obj.attr(attr.c_str()),*pChildType,chg);

		iter->second = val;
		return true;
	}

	std::string DataTypeFixDict_PyObject::ToString()const
	{
		std::stringstream ss;
		ss<<"FD{";

		for(AttributeID i = 0;i<m_dataType.GetChildCount();++i)
		{
			boost::shared_ptr<DataTypeBase> pChildType = m_dataType.GetChildTypeByID(i);
			if(!pChildType)
				return false;

			MapName2ChildObjT::const_iterator iter = m_mapName2ChildObjs.find(pChildType->GetRefName());
			if(iter == m_mapName2ChildObjs.end())
				return false;

			ss<<"'"<<m_dataType.GetChildTypeByID(i)->GetRefName()<<"':";
			bpy::str strObj(iter->second);
			if(strObj.is_none())
			{
				ss<<"None";
			}
			else
			{
				ss<<bpy::extract<char *>(strObj);
			}
			ss<<",";
		}
		ss<<"}";
		return ss.str();
	}

	bool DataTypeFixDict_PyObject::WriteToStream( BinaryOStream & oStream,bpy::object & obj,AttributeFlagT flag ) 
	{
		oStream<< AttributeID(m_mapName2ChildObjs.size());
		for(AttributeID id = 0;id < m_dataType.GetChildCount();++id)
		{
			DataTypeBasePtr childType = m_dataType.GetChildTypeByID(id);
			MapName2ChildObjT::iterator iter = m_mapName2ChildObjs.find(childType->GetRefName());
			if(iter == m_mapName2ChildObjs.end())
				continue;
			oStream<<id;
			childType->WriteToStream(oStream,iter->second,flag);
		}
		return true;
	}

	AttributeID DataTypeFixDict_PyObject::py_len( boost::python::object obj )
	{
		DataTypeFixDict_PyObject & dict = bpy::extract<DataTypeFixDict_PyObject&>(obj);
		return AttributeID(dict.m_mapName2ChildObjs.size());
	}

	bpy::object DataTypeFixDict_PyObject::py_getattr( boost::python::object obj, std::string attr )
	{
		DataTypeFixDict_PyObject & dict = bpy::extract<DataTypeFixDict_PyObject&>(obj);
		MapName2ChildObjT::iterator iter = dict.m_mapName2ChildObjs.find(attr);
		if(iter == dict.m_mapName2ChildObjs.end())
		{
			//pySetErr
			throw bpy::error_already_set();
		}
		return iter->second;
	}
	bool DataTypeFixDict_PyObject::py_contains( boost::python::object obj, std::string attr )
	{
		DataTypeFixDict_PyObject & dict = bpy::extract<DataTypeFixDict_PyObject&>(obj);
		return dict.m_mapName2ChildObjs.find(attr) != dict.m_mapName2ChildObjs.end();
	}

}
