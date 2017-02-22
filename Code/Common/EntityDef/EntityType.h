#ifndef ENTITY_DEF_H
#define ENTITY_DEF_H

#include "EntityInfo.h"
#include "EntityMethods.h"
#include "DataTypes/DataTypeFixedDict.h"

namespace Aurora
{
	class EntityType;
	typedef boost::shared_ptr<EntityType> EntityTypePtr;

	class EntityType : public DataTypeFixDict
	{
		friend class EntityTypeManager;
	public:
		EntityType(const std::string & name);
		~EntityType();

		EntityTypeID	GetTypeID()const{return m_tid;}
		const std::string & GetName()const{return m_name;}

		//////////////////////////////////////////////////////////////////////////
		//º”‘ÿœ‡πÿ;
		bool LoadFromXML(tinyxml2::XMLHandle rootXml,std::set<std::string> & childNames,bool bLoadPython,bool bInheritAllFromParent);
		void InheritAll(EntityType & child)const;

		bool HasClientMethod(const std::string & methodName)const;
		bool HasBaseMethod(const std::string & methodName)const;
		bool HasCellMethod(const std::string & methodName)const;

		const EntityAoIInfo		   & GetAoIInfo()const{return m_aoiInfo;}
		const EntityVolatileInfo   & GetVolatileInfo()const{return m_volatileInfo;}

		const EntityMethodGroup & GetClientMethods()const{return m_clientMethods;}
		const EntityMethodGroup & GetBaseMethods()const{return m_baseMethods;}
		const EntityMethodGroup & GetCellMethods()const{return m_cellMethods;}

		boost::python::object  GetClass()const{return m_pyClass;}
		boost::python::object  GetControlledClass()const{return m_pyControlledClass;}

		void NewDict(bpy::object d,AttributeFlagT flag,AttributeOwnerBase * pOwner)const;

		//override DataTypeBase;
		virtual void AddToMD5(MD5 & md5)const;
		virtual bool WriteToStream(BinaryOStream & oStream,bpy::object & obj,AttributeFlagT flag)const;
		virtual AttributeOwnerBase * AsAttributeOwner(bpy::object obj);
		//~override DataTypeBase;

		bool ReadFromStream(BinaryIStream & iStream,bpy::object & ownerObj);

		bool OnSingleAttributeChange(bpy::object & ownerObj,const VAttributesT & path,AttributeID leafAttrID,BinaryIStream & is,bool bCallback);
	private:
		void inheritFromAllMyParents(EntityType & child)const;

		void SetEntityTypeID(EntityTypeID id){m_tid = id;}
	private:
		const std::string m_name;
		EntityTypeID	m_tid;

		typedef std::vector<boost::shared_ptr< EntityType >  >	VParentTypesT;
		VParentTypesT				m_vParents;
		EntityAoIInfo				m_aoiInfo;
		EntityVolatileInfo			m_volatileInfo;
		EntityMethodGroup			m_clientMethods;
		EntityMethodGroup			m_baseMethods;
		EntityMethodGroup			m_cellMethods;

		boost::python::object		m_pyModule;
		boost::python::object		m_pyClass;
		boost::python::object		m_pyControlledClass;
	};
}

#endif
