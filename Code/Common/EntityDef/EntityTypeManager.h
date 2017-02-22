#ifndef ENTITY_TYPE_MANAGER_H
#define ENTITY_TYPE_MANAGER_H

#include <boost/serialization/singleton.hpp>
#include "Constants.h"

namespace Aurora
{
	class EntityDef;
	class EntityType;
	class EntityTypeManager : public boost::serialization::singleton<EntityTypeManager>
	{
		friend class EntityType;
	public:
		EntityTypeManager();
		~EntityTypeManager();

		bool Init(const std::string & scriptPath,bool bLoadPython,bool bInheritAllParent);

		boost::shared_ptr<EntityType> GetEntityType(uint32 idx)const;
		boost::shared_ptr<EntityType> GetEntityType(const std::string & typeName)const;

		boost::shared_ptr<EntityType> GetAccountEntityType()const
		{
			return GetEntityType(GetAccountTypeName());
		}
		inline uint32 GetNumEntityTypes()const{return uint32(m_vEntityTypes.size());}

	private:
		bool LoadEntityType(const std::string & name,std::set<std::string> & childs ,bool bLoadPython,bool bInheritAllParent);
		void AddToMD5(MD5 & md5)const;

		boost::shared_ptr<EntityType> GetLoadedEntityType(const std::string & typeName)const;
	private:
		typedef std::vector<boost::shared_ptr<EntityType> > VEntityTypes;
		typedef std::map<std::string,boost::shared_ptr<EntityType> > MapStr2EntityTypeT;
		MapStr2EntityTypeT m_mapLoadedTypes;

		VEntityTypes m_vEntityTypes;		
		MapStr2EntityTypeT m_mapEntityTypes;
	};
}

#endif
