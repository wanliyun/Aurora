#ifndef ENTITY_DEF_METHODS_H
#define ENTITY_DEF_METHODS_H

#include "DataTypes/DataTypeFixedDict.h"
#include "DataTypes/MetaDataTypeBase.h"

namespace Aurora
{
	class MD5;
	class EntityDefMethodGroup;
	class DataTypeBase;

	class EntityMethod : public DataTypeBase
	{
		friend class EntityDefMethodGroup;
	public:
		EntityMethod();
		~EntityMethod();

		inline const std::string & GetName()const{return GetRefName();}
		bool LoadFromXML(tinyxml2::XMLElement  & xmlEle);

		bool IsExposed()const{return m_bIsExposed;}

		//override DataTypeBase;
		virtual bpy::object NewObj(AttributeFlagT flag)const;
		virtual bpy::object NewObj(BinaryIStream & iStream)const;

		virtual void AddToMD5(MD5 & md5)const;
		virtual bool WriteToStream(BinaryOStream & oStream,bpy::object & obj,AttributeFlagT flag)const;
		//~override DataTypeBase;
	private:
		EntityMethod & operator = (const EntityMethod & );
		EntityMethod(const EntityMethod & other);
	private:
		bool	m_bIsExposed;

		VDataTypeT m_vArgs;
		Name2IDMapT m_name2Id;
	};

	class EntityMethodGroup
	{
	public:
		EntityMethodGroup();
		~EntityMethodGroup();

		boost::shared_ptr<EntityMethod> GetMethod(const std::string & name,MethodID & idx)const;
		boost::shared_ptr<EntityMethod> GetMethod(MethodID  idx)const;

		bool HasMethod(const std::string & name)const;

		bool CopyMethods(EntityMethodGroup & target)const;

		bool LoadFromXML(tinyxml2::XMLHandle  handle);
		void AddToMD5(MD5 & md5)const;

	private:
		void rebuildMap();
	private:
		typedef std::vector<boost::shared_ptr<EntityMethod> > VEntityMethodT;
		VEntityMethodT m_vMethods;

		typedef std::map<std::string,MethodID> MapName2IDT;
		MapName2IDT m_mapMethods; 
	};
}

#endif
