#include "pch.h"
#include "EntityType.h"
#include "EntityTypeManager.h"
#include "binary_stream.h"
#include "memory_stream.h"
#include "pyscript/PyScript.h"
#include "DataTypes/MetaDataTypeBase.h"

namespace Aurora
{
	EntityType::EntityType(const std::string & name)
		:DataTypeFixDict(0)
		,m_name(name)
	{

	}

	EntityType::~EntityType()
	{
	}

	bool EntityType::LoadFromXML( tinyxml2::XMLHandle rootXml ,std::set<std::string> & childNames,bool bLoadPython,bool bInheritAllFromParent)
	{
		bool bIsParent = !childNames.empty();

		//加载Parents;
		if(rootXml.FirstChildElement("Parents").FirstChildElement("Parent").ToElement())
		{
			std::set<std::string> childsAndMe(childNames);
			childsAndMe.insert(m_name);

			std::set<std::string> myParents;
			tinyxml2::XMLElement * pEle = rootXml.FirstChildElement("Parents").FirstChildElement("Parent").ToElement();
			for(;pEle;pEle=pEle->NextSiblingElement("Parent"))
			{
				//有效性;
				const char * pSZPath = pEle->Attribute("path");
				if(!pSZPath)
				{
					Log_ErrorF("bad parent of entity[%s]\n",m_name.c_str());
					continue;
				}

				//重复的parent;
				const std::string parentName(pSZPath);
				if(myParents.find(parentName) != myParents.end())
				{
					Log_ErrorF("duplex parent[%s]\n",parentName.c_str());
					continue;
				}
				myParents.insert(parentName);

				//防止出现环路继承;
				if(childsAndMe.find(parentName) != childsAndMe.end() )
				{
					Log_ErrorF("bad parent[%s] of entity[%s] CIRCLE\n",parentName.c_str(),m_name.c_str());
					continue;
				}

				//加载Parent;
				EntityTypeManager::get_mutable_instance().LoadEntityType(parentName,childsAndMe,bLoadPython,bInheritAllFromParent);

				boost::shared_ptr< EntityType > pParent = EntityTypeManager::get_mutable_instance().GetLoadedEntityType(parentName);

				if(!pParent)
				{
					Log_ErrorF("entity[%s] Can not find parent[%s]\n",m_name.c_str(),parentName.c_str());
					continue;
				}
				m_vParents.push_back(pParent);
			}
		}

		//如果是真正的实体,则继承所有Parent的属性和方法;
		if(!bIsParent)
		{
			inheritFromAllMyParents(*this);
		}

		//加载AoI;
		m_aoiInfo.LoadFromXML(rootXml.FirstChildElement("AOI").ToElement());
		m_volatileInfo.LoadFromXML(rootXml.FirstChildElement("VolatileInfo"));

		//加载Attribute;
		DataTypeFixedDictMeta::BuildDictType(*this,rootXml.FirstChildElement("Attributes"));
		
		//加载方法;
		if(!m_clientMethods.LoadFromXML(rootXml.FirstChildElement("Methods").FirstChildElement("Client")))
		{
			Log_ErrorF("entity[%s] failed to load client method\n",m_name.c_str());
			return false;
		}
		if(!m_baseMethods.LoadFromXML(rootXml.FirstChildElement("Methods").FirstChildElement("Base")))
		{
			Log_ErrorF("entity[%s] failed to load base method\n",m_name.c_str());
			return false;
		}
		if(!m_cellMethods.LoadFromXML(rootXml.FirstChildElement("Methods").FirstChildElement("Cell")))
		{
			Log_ErrorF("entity[%s] failed to load cell method\n",m_name.c_str());
			return false;
		}
		return true;
	}

	void EntityType::InheritAll( EntityType & child ) const
	{
		//首先继承自己的父节点;
		inheritFromAllMyParents(child);
		
		//继承AoI info ;
		if(m_aoiInfo.m_bValid)
		{
			child.m_aoiInfo = m_aoiInfo;
		}

		//继承 volatile info ;
		if(m_volatileInfo.m_bValid)
		{
			child.m_volatileInfo = m_volatileInfo;
		}

		//继承自己的Client方法;
		m_clientMethods.CopyMethods(child.m_clientMethods);
		m_baseMethods.CopyMethods(child.m_baseMethods);
		m_cellMethods.CopyMethods(child.m_cellMethods);
	}

	bool EntityType::HasClientMethod( const std::string & methodName ) const
	{
		return m_clientMethods.HasMethod(methodName);
	}

	bool EntityType::HasBaseMethod( const std::string & methodName ) const
	{
		return m_baseMethods.HasMethod(methodName);
	}

	bool EntityType::HasCellMethod( const std::string & methodName ) const
	{
		return m_cellMethods.HasMethod(methodName);
	}

	void EntityType::AddToMD5( MD5 & md5 ) const
	{
		DataTypeFixDict::AddToMD5(md5);

		m_clientMethods.AddToMD5(md5);
		m_baseMethods.AddToMD5(md5);
		m_cellMethods.AddToMD5(md5);
	}

	void EntityType::inheritFromAllMyParents( EntityType & child ) const
	{
		for(VParentTypesT::const_iterator iter = m_vParents.begin();iter!=m_vParents.end();++iter)
		{
			(*iter)->InheritAll(child);
		}
	}

	void EntityType::NewDict(bpy::object d, AttributeFlagT flag ,AttributeOwnerBase * pOwner) const
	{
		for(AttributeID i = 0;i<GetChildCount();++i)
		{
			DataTypeBasePtr childType = GetChildTypeByID(i);
			if(!childType->HasAnyFlag(flag))
				continue;

			bpy::object obj = childType->NewObj(flag);
			childType->OnAttach(obj,pOwner,i);
			d[childType->GetRefName().c_str()] = obj;
		}
	}

	bool EntityType::WriteToStream( BinaryOStream & oStream,bpy::object & obj,AttributeFlagT flag ) const
	{
		uint16 nAttriButeCount = 0;
		for(AttributeID id = 0;id <GetChildCount();++id)
		{
			DataTypeBasePtr childType = GetChildTypeByID(id);
			if(!childType->HasAnyFlag(flag))
				continue;
			nAttriButeCount++;
			
		}

		oStream<<nAttriButeCount;
		for(AttributeID id = 0;id < GetChildCount();++id)
		{
			DataTypeBasePtr childType = GetChildTypeByID(id);
			if(!childType->HasAnyFlag(flag))
				continue;

			oStream<<id;
			bpy::object objChild = obj.attr( childType->GetRefName().c_str() );
			childType->WriteToStream(oStream,objChild,flag);
		}

		return true;
	}

	bool EntityType::ReadFromStream( BinaryIStream & iStream,bpy::object & ownerObj )
	{
		uint16 nAttriButeCount = 0;
		iStream>>nAttriButeCount;
		if(iStream.error())
			return false;
		AttributeOwnerBase & owner = bpy::extract<AttributeOwnerBase &>(ownerObj);
		AttributeID id;
		for(uint16 nAttr = 0;nAttr < nAttriButeCount;++nAttr)
		{
			iStream>>id;

			DataTypeBasePtr childType = GetChildTypeByID(id);
			if(!childType)
			{
				Log_ErrorF("Can not find type by id=[%d]",id);
				return false;
			}
			bpy::object oldValue = ownerObj.attr(childType->GetRefName().c_str());
			bpy::object newValue = childType->NewObj(iStream);
			PyScript::SetAttr(ownerObj,childType->GetRefName(),newValue);
			childType->OnDetach(oldValue);
			childType->OnAttach(newValue,&owner,id);
		}
		return !iStream.error();
	}

	bool EntityType::OnSingleAttributeChange( bpy::object & ownerObj,const VAttributesT & path,AttributeID leafAttrID,BinaryIStream & is,bool bCallback )
	{
		std::string strFuncName= std::string("set");
		DataTypeBase * pOwnerType = this;
		bpy::object owner = ownerObj;
		for(size_t i = path.size() - 1;pOwnerType;--i)
		{
			pOwnerType = pOwnerType->GetChildTypeByID(path[i]).get();
			owner = owner.attr(pOwnerType->GetRefName().c_str());

			strFuncName+="_";
			strFuncName+=pOwnerType->GetRefName();

			if(i == 0)
				break;
		}

		if(!pOwnerType)
		{
			Log_ErrorF("bad Path\n");
			return false;
		}

		DataTypeBasePtr childType = pOwnerType->GetChildTypeByID(leafAttrID);
		if(!childType)
		{
			Log_ErrorF("bad leafID\n");
			return false;
		}
		strFuncName+="_";
		strFuncName+=childType->GetRefName();

		bpy::object oldValue = owner.attr(childType->GetRefName().c_str());
		bpy::object newValue = childType->NewObj(is);

		//调用回调函数;
		{
			if(PyObject_HasAttrString(ownerObj.ptr(),strFuncName.c_str()))
			{
				PyObject* pyResult = PyObject_CallMethod(ownerObj.ptr(),const_cast<char*>(strFuncName.c_str()),const_cast<char*>("O"),newValue.ptr());
				if(pyResult != NULL)
				{
					Py_DECREF(pyResult);
				}
				else
				{
					PyErr_PrintEx(0);	
				}
			}
		}

		PyScript::SetAttr(owner,childType->GetRefName(),newValue);
		childType->OnDetach(oldValue);
		childType->OnAttach(newValue,pOwnerType->AsAttributeOwner(owner),leafAttrID);
		return true;
	}

	AttributeOwnerBase * EntityType::AsAttributeOwner( bpy::object obj )
	{
		AttributeOwnerBase & owner = bpy::extract<AttributeOwnerBase &>(obj);
		return &owner;
	}


// 	void EntityType::WriteToStream( BinaryOStream & oStream,bpy::object & ownerObj,EntityDefAttribute::FLAG_TYPE flg )const
// 	{
// 		MemoryOStream tmpStream;
// 		uint16 idx = 0;
// 		uint16 nCount = 0;
// 		for(VAttributeT::const_iterator iter = m_vAttriButes.begin();iter != m_vAttriButes.end();++iter,++idx)
// 		{
// 			MY_ASSERT(*iter);
// 			const EntityDefAttribute & attr = *(*iter);
// 			MY_ASSERT(attr.m_pDataTypeBase);
// 			if(!attr.HasAnyFlag(flg))
// 				continue;
// 			try
// 			{
// 				tmpStream<<idx;
// 				bpy::object objVal = ownerObj.attr(attr.m_strName.c_str());
// 				attr.m_pDataTypeBase->WriteToStream(tmpStream,objVal);
// 				++nCount;
// 			}
// 			catch(...)
// 			{
// 				PyErr_Print();
// 				continue;
// 			}
// 		}
// 		oStream<<nCount;
// 		oStream.addBlob(tmpStream.data(),tmpStream.size());
// 	}

// 	bool EntityType::ReadFromStreamX( BinaryIStream & iStream,bpy::object & ownerObj ,bool bCallback) const
// 	{
// 		uint16 idx = 0;
// 		uint16 nCount = 0;
// 		iStream>>nCount;
// 		bool bRet = true;
// 		for(uint16 n =0 ;n<nCount;++n)
// 		{
// 			iStream>>idx;
// 			bRet &= ReadOneFromStream(iStream,ownerObj,idx,bCallback);
// 		}
// 
// 		return bRet && !iStream.error();
// 	}
// 
// 	bool EntityType::ReadOneFromStream( BinaryIStream & iStream,bpy::object & ownerObj,AttributeID attrID ,bool bCallback) const
// 	{
// 		try
// 		{
// 			if(attrID>=m_vAttriButes.size())
// 			{
// 				Log_ErrorF(" bad idx = %u\n",attrID);
// 				return false;
// 			}
// 			MY_ASSERT(m_vAttriButes[attrID] && m_vAttriButes[attrID]->m_pDataTypeBase);
// 			const EntityDefAttribute & attr = *m_vAttriButes[attrID];
// 			bpy::object obj = attr.m_pDataTypeBase->ReadFromStream(iStream);
// 
// 			PyScript::SetAttr(ownerObj,attr.m_strName,obj,bCallback);
// 		}
// 		catch(...)
// 		{
// 			PyErr_Print();
// 			return false;
// 		}
// 		return !iStream.error();
// 	}
}
