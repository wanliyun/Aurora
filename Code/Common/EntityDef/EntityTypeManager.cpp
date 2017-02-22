#include "pch.h"
#include "EntityType.h"
#include "EntityTypeManager.h"
#include <FileSystem.h>
#include "DataTypes/MetaDataTypeBase.h"


namespace Aurora
{
	EntityTypeManager::EntityTypeManager()
	{

	}

	EntityTypeManager::~EntityTypeManager()
	{
	}

	bool EntityTypeManager::Init( const std::string & scriptPath,bool bLoadPython ,bool bInheritAllParent  )
	{
		if(!DataTypeBase::Init(std::string(GetEntityDefDir())+ GetDataTypesFileName()))
		{
			Log_CriticalF("DataType::init Failed \n");
			return false;
		}

		MY_ASSERT(GetEntityDefDir());
		std::string allEntitiesFileName = std::string(GetEntityDefDir()) + GetEntitiesFileName();
		tinyxml2::XMLDocument doc;
		std::string absFilePath = FileSystem::get_mutable_instance().FindFilePath(allEntitiesFileName);
		if(absFilePath.empty() || tinyxml2::XML_NO_ERROR != doc.LoadFile(absFilePath.c_str()))
		{
			Log_CriticalF("can not find file[%s]\n",allEntitiesFileName.c_str());
			return false;
		}
		tinyxml2::XMLHandle hDoc(&doc);
		tinyxml2::XMLElement * pEnt  = hDoc.FirstChildElement("Entities").FirstChild().ToElement();
		Log_NormalF("<<<<<<<Entities List Begin\n");
		for( pEnt; pEnt; pEnt=pEnt->NextSiblingElement() )
		{
			if(!pEnt->Value())
				continue;
			const std::string strEntityTypeName(pEnt->Value());

			Log_NormalF("\t[%s]\n",strEntityTypeName.c_str());

			std::set<std::string> childs;
			if(!LoadEntityType(strEntityTypeName,childs,bLoadPython,bInheritAllParent))
				return false;
		}
		Log_NormalF("Entities List End>>>>>>>\n");
		MD5 md5;
		AddToMD5(md5);
		MD5::Digest dig;
		md5.GetDigest(dig);
		Log_OKF("md5=[%s]\n",dig.Quote().c_str());

		return true;
	}


	void EntityTypeManager::AddToMD5( MD5 & md5 ) const
	{
		for(VEntityTypes::const_iterator iter = m_vEntityTypes.begin();iter != m_vEntityTypes.end();++iter)
			(*iter)->AddToMD5(md5);
	}

	bool EntityTypeManager::LoadEntityType( const std::string & name,std::set<std::string> & childs ,bool bLoadPython,bool bInheritAllParent )
	{
		bool bAsParent = !childs.empty();
		
		if(m_mapLoadedTypes.find(name) != m_mapLoadedTypes.end())
			return true;

		boost::shared_ptr<EntityType > pDataType;

		// Step 1: 加载本类型;
		{
			MY_ASSERT(GetEntityDefDir());
			tinyxml2::XMLDocument doc;
			std::string allEntitiesFileName = std::string(GetEntityDefDir()) + name + ".xml";
			std::string absFilePath = FileSystem::get_mutable_instance().FindFilePath(allEntitiesFileName);
			if(absFilePath.empty() )
			{
				Log_CriticalF("can not find file[%s]\n",allEntitiesFileName.c_str());
				return false;
			}

			if(tinyxml2::XML_NO_ERROR != doc.LoadFile(absFilePath.c_str()))
			{
				Log_CriticalF("bad file[%s]\n",allEntitiesFileName.c_str());
				return false;
			}

			pDataType.swap( boost::shared_ptr<EntityType>( new EntityType(name) ) );
			m_mapLoadedTypes.insert( std::make_pair(name,pDataType));

			tinyxml2::XMLHandle hDoc(&doc);
			if(!pDataType->LoadFromXML(hDoc.FirstChildElement("root"),childs,false,bInheritAllParent))
			{
				Log_Error("failed to load Entity [%s]\n",name.c_str());
				return false;
			}
		}
		
		// Step 2: load python scripts;
		if(bAsParent )
			return true;

		pDataType->SetEntityTypeID(EntityTypeID(m_vEntityTypes.size()));
		m_vEntityTypes.push_back(pDataType);
		m_mapEntityTypes.insert( std::make_pair(pDataType->GetName(),pDataType) );
		


		if(! bLoadPython)
			return true;
		try
		{
			boost::python::object entityModule = boost::python::import(name.c_str());
			boost::python::object entityClass = entityModule.attr(name.c_str());

			boost::python::object controledEntityClass;

			try
			{
				std::string ControllClassName = std::string("Controlled") + name;
				boost::python::object tmpControllClass = entityModule.attr( ControllClassName.c_str());
				if(PyType_IsSubtype((PyTypeObject *)tmpControllClass.ptr(),(PyTypeObject *)entityClass.ptr()))
					controledEntityClass = tmpControllClass;
			}
			catch(boost::python::error_already_set)
			{
				PyErr_Clear();

			}

			if(PyType_Check(entityClass.ptr()))
			{
				pDataType->m_pyClass = entityClass;
				pDataType->m_pyModule = entityModule;
				pDataType->m_pyControlledClass = controledEntityClass;
			}
		}
		catch(...)
		{
			if (PyErr_Occurred())  
				PyErr_Print();
		}
		return true;
	}

	boost::shared_ptr<EntityType> EntityTypeManager::GetEntityType( uint32 idx )const
	{
		if(idx >= m_vEntityTypes.size())
			return 0;
		return m_vEntityTypes[idx];
	}

	boost::shared_ptr<EntityType> EntityTypeManager::GetEntityType(const std::string & typeName)const
	{
		MapStr2EntityTypeT::const_iterator iter = m_mapEntityTypes.find(typeName);
		if(iter == m_mapEntityTypes.end())
			return 0;
		return iter->second;
	}

	boost::shared_ptr<EntityType> EntityTypeManager::GetLoadedEntityType( const std::string & typeName ) const
	{
		MapStr2EntityTypeT::const_iterator iter  = m_mapLoadedTypes.find(typeName);
		if(iter == m_mapLoadedTypes.end())
			return 0;
		return iter->second;
	}



}

