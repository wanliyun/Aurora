
#include "pch.h"
#include "CellEntity.h"
#include <EntityDef/EntityType.h>
#include <EntityDef/DataTypes/DataTypeBase.h>
#include "../baseApp/BaseInternInterface.h"
#include "Apps/CellApp_BaseApp.h"
#include "Space/Cell.h"
#include "Space/CellManager.h"
#include "../../Clients/Client/ClientInterface.h"
#include "Entity/CellEntityManager.h"
#include "Entity/CellEntityAOIManager.h"
#include <memory_stream.h>


namespace Aurora
{
	CellEntity::CellEntity( XEntityID id,EntityTypePtr pType,AttributeFlagT flag,AppID baseAppID )
		:PosRotEntityBase(pType,XID2ID(id),XID2AppID(id))
		,DataTypeFixDict_PyObject(pType.get())
		,m_AOIManager(*this)
		,m_pEntityType__(pType.get())
		,m_nBaseAppID(baseAppID)
		,m_bIsDestroying(false)
		,m_volatileStamp(0)
	{

	}

	CellEntity::~CellEntity()
	{

	}

	const EntityType & CellEntity::GetType() const
	{
		MY_ASSERT(m_pEntityType__ != 0);
		return * m_pEntityType__;
	}

// 	void CellEntity::onPreSetAttribute( boost::python::object thisObj, const std::string & str,boost::python::object val )
// 	{
// 		const EntityDef & def= GetType();
// 		AttributeID aid =  def.GetAttributeID(str);
// 		const EntityDefAttribute * pAttr = def.GetAttribute(aid);
// 		if(!pAttr)
// 			return;
// 		MY_ASSERT(pAttr->m_pDataTypeBase);
// 
// 		if( !pAttr->IsCellOwn() )
// 		{
// 			Log_ScriptF("RWARNING:changing non cell own attribute [%s:%s]\n",GetType().GetName().c_str(),pAttr->m_strName.c_str());
// 			return;
// 		}
// 
// 		MemoryOStream os;
// 		pAttr->m_pDataTypeBase->WriteToStream(os,val);
// 
// 		if(GetBaseApp())
// 		{
// 			BaseInternInterface::PktOneAttributeChangeByCell newPkt;
// 			newPkt.m_eID = m_Id;
// 			newPkt.m_attrID = aid;
// 			newPkt.AppendToBuff((char*)os.data(),os.size());
// 			SendToBase(newPkt);
// 		}
// 
// 
// 		//通知其他客户端;
// 		if(pAttr->IsOtherClient())
// 		{
// 			ClientInterface::PktOneAttributeChange newPkt;
// 			newPkt.m_eID = m_Id;
// 			newPkt.m_attrID = aid;
// 			newPkt.AppendToBuff((char*)os.data(),os.size());
// 			SendToOtherClients(newPkt);
// 		}
// 
// 	}
// 
// 	void CellEntity::setattr( boost::python::object obj, std::string attr, boost::python::object val )
// 	{
// 		CellEntity & ent = bpy::extract<CellEntity &>(obj);
// 		ent.onPreSetAttribute(obj,attr,val);
// 
// 		bpy::str attr_str(attr);
// 		PyObject_GenericSetAttr(obj.ptr(), attr_str.ptr(), val.ptr());
// 	}

	void CellEntity::SendToClient(const PacketBase & pkt )
	{
		MY_ASSERT(pkt.GetHeader().GetInterface() == EPacketInterface_Client);
		CellApp_BaseApp * pBaseApp = GetBaseApp();
		if(!pBaseApp)
		{
			Log_ErrorF(" Can not find baseApp[%u]\n",m_nBaseAppID);
			return;
		}

		pBaseApp->SendPacketToClient(pkt,GetID());
	}


	void CellEntity::SendToOtherClients( const PacketBase & pkt )
	{
		for(std::list<EntityID>::const_iterator iter = m_Listners.begin();iter != m_Listners.end();++iter)
		{
			CellEntity * pEnt = CellEntityManager::get_mutable_instance().GetEntity(*iter);
			if(!pEnt)
			{
				Log_ErrorF("can not find entity[%u]\n",*iter);
				continue;
			}
			pEnt->SendToClient(pkt);
		}
	}



	void CellEntity::SendToBase( const PacketBase & pkt )
	{
		CellApp_BaseApp * pBaseApp = GetBaseApp();
		if(!pBaseApp)
		{
			Log_ErrorF(" Can not find baseApp[%u]\n",m_nBaseAppID);
			return;
		}

		pBaseApp->GetConn().SendPacket(pkt);
	}


	CellApp_BaseApp * CellEntity::GetBaseApp()
	{
		return CellApp_BaseAppManager::get_mutable_instance().GetBaseApp(m_nBaseAppID);
	}

	Cell * CellEntity::GetCell()
	{
		return CellManager::get_mutable_instance().GetCell(m_fullCellID);
	}

	void CellEntity::Init( const EntityType * pEntityType,const SFullCellID & fullID )
	{
		MY_ASSERT(m_pEntityType__ == 0);
		m_pEntityType__ = pEntityType;
		m_fullCellID = fullID;
	}

	void CellEntity::Destroy()
	{
		m_bIsDestroying = true;
	}

	void CellEntity::OnTick( float fDelta )
	{
		m_AOIManager.OnTick(fDelta);
	}

	void CellEntity::RegisterListener( EntityID id )
	{
		if(std::find(m_Listners.begin(),m_Listners.end(),id)==m_Listners.end())
			m_Listners.push_back(id);
	}

	void CellEntity::DeRegisterListener( EntityID id )
	{
		m_Listners.remove(id);
	}

	void CellEntity::CallBaseMethod( std::string  strName,bpy::dict  args )
	{
		MethodID idx;
		boost::shared_ptr<EntityMethod> pMethod = GetType().GetBaseMethods().GetMethod(strName,idx);
		if(!pMethod)
		{
			Log_ScriptF("entity type [%s] has no base method[%s]\n",GetType().GetName().c_str(),strName.c_str());
			return ;
		}

		BaseInternInterface::PktMethodCallFromCell pktToBase;
		pktToBase.m_eID = GetID();
		pktToBase.m_mID = idx;

		MemoryOStream os;
		pMethod->WriteToStream(os,args,DataTypeBase::EFlag_All);
		pktToBase.AppendToBuff(os.data(),os.size());
		SendToBase(pktToBase);
	}

	void CellEntity::CallCellMethod( std::string strName,bpy::dict args )
	{

	}

	void CellEntity::CallOwnClientMethod( std::string strName,bpy::dict args )
	{
		MethodID idx;
		boost::shared_ptr<EntityMethod> pMethod = GetType().GetClientMethods().GetMethod(strName,idx);
		if(!pMethod)
		{
			Log_ScriptF("entity type [%s] has no client method[%s]\n",GetType().GetName().c_str(),strName.c_str());
			return ;
		}

		ClientInterface::PktMethodCallFromCell pktToClient;
		pktToClient.m_eID = GetID();
		pktToClient.m_mID = idx;

		MemoryOStream os;
		pMethod->WriteToStream(os,args,DataTypeBase::EFlag_All);
		pktToClient.AppendToBuff(os.data(),os.size());
		SendToClient(pktToClient);
	}

	void CellEntity::CallOtherClientMethod( std::string strName,bpy::dict args )
	{
		MethodID idx;
		boost::shared_ptr<EntityMethod> pMethod = GetType().GetClientMethods().GetMethod(strName,idx);
		if(!pMethod)
		{
			Log_ScriptF("entity type [%s] has no client method[%s]\n",GetType().GetName().c_str(),strName.c_str());
			return ;
		}

		ClientInterface::PktMethodCallFromCell pktToClient;
		pktToClient.m_eID = GetID();
		pktToClient.m_mID = idx;

		MemoryOStream os;
		pMethod->WriteToStream(os,args,DataTypeBase::EFlag_All);
		pktToClient.AppendToBuff(os.data(),os.size());
		SendToOtherClients(pktToClient);
	}

	void CellEntity::CallAllClientMethod( std::string strName,bpy::dict args )
	{
		MethodID idx;
		boost::shared_ptr<EntityMethod> pMethod = GetType().GetClientMethods().GetMethod(strName,idx);
		if(!pMethod)
		{
			Log_ScriptF("entity type [%s] has no client method[%s]\n",GetType().GetName().c_str(),strName.c_str());
			return ;
		}

		ClientInterface::PktMethodCallFromCell pktToClient;
		pktToClient.m_eID = GetID();
		pktToClient.m_mID = idx;

		MemoryOStream os;
		pMethod->WriteToStream(os,args,DataTypeBase::EFlag_All);
		pktToClient.AppendToBuff(os.data(),os.size());
		SendToClient(pktToClient);
		SendToOtherClients(pktToClient);
	}

	void CellEntity::OnVolatileStampChangedByClient()
	{
		OnVolatileStampChanged();
		GetAOIManager().RefreshOwnClientVolatileStamp();
	}

}
