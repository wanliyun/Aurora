
#include "pch.h"
#include "AppManagerInterface.h"
#include <binary_stream.h>

using namespace std;

namespace Aurora
{
	namespace AppManagerInterface
	{
		BEGIN_IMPLEMENT_PACKET_FACTORY(EPacketInterface_AppManager);

		ADD_PACKET_PRODUCT(AppManagerInterface::EPacketIndex_WhoAmI					,AppManagerInterface::PktWhoAmI					);
		ADD_PACKET_PRODUCT(AppManagerInterface::EPacketIndex_LoginRquest			,AppManagerInterface::PktLoginRquest			);
		ADD_PACKET_PRODUCT(AppManagerInterface::EPacketIndex_NoticeClientLose		,AppManagerInterface::PktNoticeClientLose		);
		ADD_PACKET_PRODUCT(AppManagerInterface::EPacketIndex_CreatePlayerResponse	,AppManagerInterface::PktCreatePlayerResponse		);
		ADD_PACKET_PRODUCT(AppManagerInterface::EPacketIndex_BroadCast	,AppManagerInterface::PktBroadcastMsg		);
		ADD_PACKET_PRODUCT(AppManagerInterface::EPacketIndex_CreateEntityOnCell	,AppManagerInterface::PktCreateEntityOnCell		);

		END_IMPLEMENT_PACKET_FACTORY();


		void PktWhoAmI::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_AppType<<m_internBindAddr<<m_externBindAddr;
		}

		void PktWhoAmI::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_AppType>>m_internBindAddr>>m_externBindAddr;
		}

		const char * PktWhoAmI::GetNameByType( uint8 tp ) 
		{
			switch(tp)
			{
			case EAppType_BaseApp:return "baseApp";
			case EAppType_CellApp:return "cellApp";
			case EAppType_LoginApp:return "loginApp";
			case EAppType_DBManager:return "dbMgr";
			default:
				return "unknown";
			}
			MY_ASSERT(0);
			return "";
		}

		void PktLoginRquest::WriteToStream( BinaryOStream & oStream )const
		{
			oStream<<m_clntID<<m_strUserName<<m_strPassword<<m_strBlowfishKey;
		}

		void PktLoginRquest::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_clntID>>m_strUserName>>m_strPassword>>m_strBlowfishKey;
		}


		void PktNoticeClientLose::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_eID;
		}

		void PktNoticeClientLose::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_eID;
		}


		void PktCreatePlayerResponse::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_bSucc<<m_strAccountName;
		}

		void PktCreatePlayerResponse::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_bSucc>>m_strAccountName;
		}


		void PktBroadcastMsg::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_msg;
		}

		void PktBroadcastMsg::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_msg;
		}


		void PktCreateEntityOnCell::WriteToStream( BinaryOStream & oStream ) const
		{
			oStream<<m_id<<m_tid;
		}

		void PktCreateEntityOnCell::ReadFromStream( BinaryIStream & iStream )
		{
			iStream>>m_id>>m_tid;
		}
	}
}
