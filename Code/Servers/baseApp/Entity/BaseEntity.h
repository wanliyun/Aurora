#ifndef _BASE_ENTITY_ON_BASE_H_
#define _BASE_ENTITY_ON_BASE_H_

#include <map>
#include <boost/serialization/singleton.hpp>
#include <EntityDef/EntityBase.h>

namespace Aurora
{
	class GlobalEntityManager;
	class BaseApp_Client;
	class BaseApp_CellApp;

	class EntityType;
	class EntityDef;
	class NetworkConn;

	class PacketBase;

	class BaseEntity : public PosRotEntityBase,  public AttributeOwnerBase
	{
		friend class GlobalEntityManager;
	public:
		BaseEntity(EntityID id,EntityTypePtr pType);
		~BaseEntity();

		/////////////////////////////////////////////////////////////////////////
		//驱动;
		bool Init();
		void OnTick(float fDelta);
		void Fini();

		void Destroy();
		bool IsDestroying()const{return m_bIsDestroying;}
		
		//////////////////////////////////////////////////////////////////////////
		//场景相关;
		bool EnterSpace(SpaceID sid,SpaceChannelID chId = BAD_SPACE_CHANNEL_ID);

		//////////////////////////////////////////////////////////////////////////
		//网络连接相关;
		bool HasClient()const{return 0!= m_pClient;}
		bool HasCell()const{return 0!= m_pCellApp;}

		void				OnAttachClient(bpy::object & self,BaseApp_Client & clnt);
		void				OnDetachClient(bpy::object & self,BaseApp_Client & clnt);

		void				OnAttachCellApp(bpy::object & self,BaseApp_CellApp & cellApp);
		void				OnDetachCellApp(bpy::object & self,BaseApp_CellApp & cellApp);

		BaseApp_Client *	GetClient()const{return m_pClient;}
		BaseApp_CellApp *	GetCellApp()const{return m_pCellApp;}
		
		AppID				GetClientID()const;
		AppID				GetCellAppID()const;


		bool SendToClient(const PacketBase & pkt);
		bool SendToCell(const PacketBase & pkt);
		bool SendToGhostCells(const PacketBase & pkt);

		const std::string & GetBlowfishKey()const{return m_strBlowfishKey;}
		void SetBlowfishKey(const std::string & strKey){m_strBlowfishKey = strKey;}

		//////////////////////////////////////////////////////////////////////////
		//远程方法调用;
		void CallClientMethod(std::string strName,bpy::dict args);
		void CallBaseMethod(std::string strName,bpy::dict args);
		void CallCellMethod(std::string strName,bpy::dict args);
		
	private:
		//override AttributeOwnerBase;
		virtual void OnSingleAttributeChange(bpy::object oriVal,class DataTypeBase & dataType,SingleAttributeChange & chg);
		virtual bool OnPreSetAttr(boost::python::object obj,const std::string & attr, boost::python::object val);
		//~override AttributeOwnerBase;

	private:
		BaseApp_Client *	m_pClient;
		BaseApp_CellApp *   m_pCellApp;
		
		std::string m_strBlowfishKey;
		bool m_bIsDestroying;
	};
	typedef boost::shared_ptr<BaseEntity>  BaseEntityPtr;
}

#endif
