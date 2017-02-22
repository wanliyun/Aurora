
#include "pch.h"
#include "EntityBase.h"
#include "EntityType.h"

namespace Aurora
{
	EntityBase::EntityBase(EntityTypePtr pType, EntityID id,AppID baseAppID )
		:m_pType(pType)
		,m_Id(id)
		,m_ownerBaseAppID(baseAppID)
	{

	}

	EntityTypeID EntityBase::GetTypeID() const
	{
		return m_pType->GetTypeID();
	}
}
