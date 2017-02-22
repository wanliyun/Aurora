
#include "pch.h"
#include "BaseEntity.h"
#include "Apps/BaseApp_Client.h"
#include <EntityDef/EntityType.h>
#include <EntityDef/EntityTypeManager.h>
#include "../../Clients/Client/ClientInterface.h"
#include "memory_stream.h"
#include "BaseApp.h"
#include "Apps/BaseApp_CellApp.h"
#include "../cellApp/CellInterface.h"
#include <EntityDef/DataTypes/DataTypeBase.h>

namespace Aurora
{
	BaseEntity::BaseEntity(EntityID id,EntityTypePtr pType )
		:PosRotEntityBase(pType,id,BaseApp::get_mutable_instance().GetAppID())
		,m_pClient(0)
		,m_pCellApp(0)
		,m_bIsDestroying(false)
	{
		Log_NormalF("id=%u\n",id);
	}


	BaseEntity::~BaseEntity()
	{
		Log_NormalF("id=%u\n",m_Id);
	}


	bool BaseEntity::Init(  )
	{
		return true;
	}

	void BaseEntity::OnTick( float fDelta )
	{

	}

	void BaseEntity::Fini()
	{

	}


	bool BaseEntity::EnterSpace( SpaceID sid,SpaceChannelID chId /*= BAD_SPACE_CHANNEL_ID*/ )
	{
		Log_NormalF("sid[%u]chid[%u]\n",sid,chId);
		SFullCellID targetCellID;
		SAllServerSpaceInfo::MapSpaceT &mapSpace = BaseApp::get_mutable_instance().GetAllSpaceInfo().m_mapSpaces;
		if(mapSpace.find(sid) == mapSpace.end())
		{
			Log_ErrorF("Can not find sid[%u]\n",sid);
			return false;
		}
		SSpaceInfo & space_info = mapSpace[sid];
		targetCellID.m_spaceId = sid;
		if(space_info.m_mapChannels.empty())
		{
			Log_ErrorF("Space sid[%u] has no channel\n",sid);
			return false;
		}
		SChannelInfo * pChannelInfo = 0;
		if(space_info.m_mapChannels.find(chId) == space_info.m_mapChannels.end())
		{
			int nidx = rand() % (int(space_info.m_mapChannels.size()));
			int i= 0;
			for(SSpaceInfo::MapChannelT::iterator iter = space_info.m_mapChannels.begin();iter != space_info.m_mapChannels.end();++iter,++i)
			{
				if(i == nidx)
				{
					pChannelInfo = &(iter->second);
					targetCellID.m_channelId = iter->first;
					break;
				}
			}
		}
		else
		{
			pChannelInfo =  &(space_info.m_mapChannels[chId]);
			targetCellID.m_channelId = chId;
		}
		MY_ASSERT(pChannelInfo);
		const SCellInfo * pCellInfo = pChannelInfo->GetCellInfo(0);
		if(!pCellInfo)
		{
			Log_ErrorF("Can not find cell info\n");
			return false;
		}
		if(pCellInfo->m_cellAppId == BAD_APP_ID)
		{
			Log_ErrorF("cell has no cell app yet\n");
			return false;
		}
		
		targetCellID.m_cellId = 0;

		BaseApp_CellApp * pCellApp = BaseApp_CellAppManager::get_mutable_instance().GetCellApp(pCellInfo->m_cellAppId);
		if(!pCellApp)
		{
			Log_ErrorF("can not find cellapp[%u]\n",pCellInfo->m_cellAppId);
			return false;
		}

		CellInterface::PktCreateEntityOnCell pkt;
		pkt.m_id = m_Id;
		pkt.m_tid = GetTypeID();
		pkt.m_fullCellID = targetCellID;
		m_pCellApp = pCellApp;
		pCellApp->SendPacket(pkt);
		return true;
	}

	void BaseEntity::Destroy()
	{
		m_bIsDestroying = true;
	}

	void BaseEntity::CallClientMethod( std::string strName,bpy::dict args )
	{
		MethodID idx;
		boost::shared_ptr<EntityMethod> pMethod = GetType()->GetClientMethods().GetMethod(strName,idx);
		if(!pMethod)
		{
			Log_ScriptF("entity type [%s] has no client method[%s]\n",GetType()->GetName().c_str(),strName.c_str());
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

	void BaseEntity::CallCellMethod( std::string strName,bpy::dict args )
	{
		MethodID idx;
		boost::shared_ptr<EntityMethod> pMethod = GetType()->GetCellMethods().GetMethod(strName,idx);
		if(!pMethod)
		{
			Log_ScriptF("entity type [%s] has no cell method[%s]\n",GetType()->GetName().c_str(),strName.c_str());
			return ;
		}
		if(!pMethod->IsExposed())
		{
			Log_ScriptF("entity type [%s] cellmethod[%s] is not exposed to client\n",GetType()->GetName().c_str(),strName.c_str());
			return ;
		}

		CellInterface::PktMethodCallFromClient pkt;
		pkt.m_eID = m_Id;
		pkt.m_mID = idx;

		MemoryOStream os;
		pMethod->WriteToStream(os,args,DataTypeBase::EFlag_All);
		pkt.AppendToBuff(os.data(),os.size());
		SendToCell(pkt);
	}

	bool BaseEntity::SendToClient( const PacketBase & pkt )
	{
		if(!m_pClient)
		{
			Log_ErrorF("No ClientApp entityid=[%u]\n",m_Id);
			return false;
		}
		m_pClient->SendPacket(pkt);
		return true;
	}

	bool BaseEntity::SendToCell( const PacketBase & pkt )
	{
		if(!m_pCellApp)
		{
			Log_ErrorF("No Cell entityid=[%u]\n",m_Id);
			return false;
		}

		m_pCellApp->SendPacket(pkt);
		return true;
	}

	bool BaseEntity::SendToGhostCells( const PacketBase & /*pkt*/ )
	{
		return true;
	}

	void BaseEntity::OnAttachClient( bpy::object & self,BaseApp_Client & clnt )
	{
		MY_ASSERT(0 == m_pClient);
		m_pClient = &clnt;

		clnt.AddControlEntity(GetID());
		
		//发送PktBasePlayerCreate;
		{
			ClientInterface::PktBasePlayerCreate newPkt;
			newPkt.m_id = GetID();
			newPkt.m_tid = GetTypeID();
			MemoryOStream os;
			GetType()->WriteToStream(os,self,DataTypeBase::EFlag_OwnClient);
			newPkt.AppendToBuff((char*)os.data(),os.size());
			SendToClient(newPkt);
		}

		//通知Python脚本层;
		try
		{
			self.attr("OnGetClient")();
		}
		catch(...)
		{
			PyErr_Print();
		}
	}

	void BaseEntity::OnDetachClient( bpy::object & self,BaseApp_Client & clnt)
	{
		//通知Python脚本层;
		try
		{
			self.attr("OnLoseClient")();
		}
		catch(...)
		{
			PyErr_Print();
		}

		clnt.DelControlEntity(GetID());
		
		if(HasCell())
		{
			CellInterface::PktDestroyEntityOnCell newPkt;
			newPkt.m_id = GetID();
			SendToCell(newPkt);
		}

		
		MY_ASSERT(0 != m_pClient);
		m_pClient = 0;
	}

	void BaseEntity::OnAttachCellApp( bpy::object & self,BaseApp_CellApp & cellApp )
	{

	}

	void BaseEntity::OnDetachCellApp( bpy::object & self,BaseApp_CellApp & cellApp )
	{

	}

	void BaseEntity::OnSingleAttributeChange( bpy::object oriVal,class DataTypeBase & dataType,SingleAttributeChange & chg )
	{
		if(HasClient() && dataType.IsOwnClient() )//同步到Client;
		{
			MemoryOStream os;
			dataType.WriteToStream(os,chg.m_newVal,DataTypeBase::EFlag_OwnClient);

			ClientInterface::PktOneAttributeChange newPkt;
			newPkt.m_eID = m_Id;
			newPkt.m_attrID = dataType.GetRefId();
			newPkt.m_path = chg.m_vPath;
			newPkt.AppendToBuff((char*)os.data(),os.size());
			SendToClient(newPkt);
		}

		if(HasCell() && (dataType.IsRealCell() || dataType.IsGhostCell()) )//同步到Cell;
		{
			CellInterface::PktOneAttributeChange newPkt;
			newPkt.m_eID = m_Id;
			newPkt.m_attrID = dataType.GetRefId();
			//newPkt.m_path = chg.m_vPath;
			if(dataType.IsRealCell())
			{
				MemoryOStream os;
				dataType.WriteToStream(os,chg.m_newVal,DataTypeBase::EFlag_RealCell);

				newPkt.AppendToBuff((char*)os.data(),os.size());
				SendToCell(newPkt);
			}
			if(dataType.IsGhostCell())
			{
				MemoryOStream os;
				dataType.WriteToStream(os,chg.m_newVal,DataTypeBase::EFlag_GhostCell);

				newPkt.AppendToBuff((char*)os.data(),os.size());
				SendToGhostCells(newPkt);
			}
		}
	}

	bool BaseEntity::OnPreSetAttr( boost::python::object obj,const std::string & attr, boost::python::object val )
	{
		BaseEntity & ent = bpy::extract<BaseEntity &>(obj);

		DataTypeBasePtr pChildType = ent.GetType()->GetChildTypeByName(attr);
		if(!pChildType)
			return false;

		if(pChildType->IsBaseOnly())
			return false;

		SingleAttributeChange chg;
		chg.m_index = pChildType->GetRefId();
		chg.m_newVal = val;
		ent.OnSingleAttributeChange(obj,*pChildType.get(),chg);
		return false;
	}
}
