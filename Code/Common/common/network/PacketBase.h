

#ifndef __PACKET_BASE_H__
#define __PACKET_BASE_H__

#include "Types.h"
#include <boost/serialization/singleton.hpp>
#include <map>
#include <string>
#include "MySocketAPI.h"

namespace Aurora
{
	class BinaryIStream;
	class BinaryOStream;

	enum EPacketInterface
	{
		EPacketInterface_Dummy,	//错误;
		EPacketInterface_Client,	//客户端;
		EPacketInterface_LoginExtern,
		EPacketInterface_LoginIntern,
		EPacketInterface_BaseExtern,
		EPacketInterface_BaseIntern,
		EPacketInterface_Cell,
		EPacketInterface_DBManager,
		EPacketInterface_AppManager,
	};

	struct SPacketHeader
	{
	public:
		SPacketHeader(EPacketInterface eIntface,uint16 idx):m_nPacketTypeID( uint16(eIntface) << 12 | idx)
		{
		}

		EPacketInterface GetInterface()const{return  EPacketInterface(m_nPacketTypeID >> 12);}
		uint16 GetIndex()const{return m_nPacketTypeID & 0xfff;}

		uint16 GetTypeID()const{return m_nPacketTypeID ;}

		void WriteToStream(BinaryOStream & data)const;
		void ReadFromStream(BinaryIStream & data);
	private:
		uint16			m_nPacketTypeID;	//本Packet的类型;前4位EPacketInterface，后12位为Index;
	};

	class PacketBase
	{
	public:

		static const uint16 MAX_PACKET_LEN = 2048;	//包含2位长度;

		explicit PacketBase( EPacketInterface eIntface,uint16 idx ):____header(eIntface,idx)
		{

		}
		virtual ~PacketBase( )
		{
	
		}

		virtual void WriteToStream(BinaryOStream & oStream)const = 0;
		virtual void ReadFromStream(BinaryIStream & iStream) = 0;
		const SPacketHeader & GetHeader()const{return ____header;}

		
		void AppendToBuff(const char *pData,uint32 dataLen);
		void AppendToBuff(const void *pData,uint32 dataLen){AppendToBuff((const char*)pData,dataLen);}
		uint32 GetBuffLen()const{return uint32(_____buff.size());}
		const char * GetBuff()const{return _____buff.c_str();}
	protected:
		void WriteBuffToStream(BinaryOStream & oStream)const;
		void ReadBuffFromStream(BinaryIStream & iStream);
	protected:
		const SPacketHeader ____header;
	private:
		std::string _____buff;
	};


	struct IPacketFactory;
	class PacketFactoryManager : public boost::serialization::singleton<PacketFactoryManager>
	{
	public:
		PacketFactoryManager(){}
		~PacketFactoryManager();

		PacketBase * NewPacket(uint16 typeID);
		void DeletePacket(PacketBase * pPacket);

		void RegisterFactory(EPacketInterface eInterface,IPacketFactory * pFactory);
		void DeregisterFactory(IPacketFactory * pFactory);

	private:
		std::map<EPacketInterface,IPacketFactory * > m_mapFactories;
	};

	struct IPacketFactory
	{
		virtual ~IPacketFactory(){}

		virtual PacketBase * NewPacket(uint16 index) = 0;
		virtual void DeletePacket(PacketBase * pPacket) = 0;
	};

#define DECLARE_PACKET_FACTORY()	bool InitPacketFactory()

#define BEGIN_IMPLEMENT_PACKET_FACTORY(INTERFACE_NAME)\
	class MyPacketFactory : public IPacketFactory, public boost::serialization::singleton<MyPacketFactory>\
	{\
	private:\
		virtual PacketBase * NewPacket(uint16 idx);\
		virtual void DeletePacket(PacketBase * pPacket);\
	};\
	bool InitPacketFactory()\
	{\
		MyPacketFactory &inst = MyPacketFactory::get_mutable_instance();\
		PacketFactoryManager::get_mutable_instance().RegisterFactory(INTERFACE_NAME,&inst);\
		return true;\
	}\
	void MyPacketFactory::DeletePacket(PacketBase * pPacket)\
	{\
		SAFE_DELETE(pPacket);\
	}\
	PacketBase * MyPacketFactory::NewPacket(uint16 idx)\
	{\
		PacketBase * pNewPkt = 0;\
		switch(idx)\
		{\
			default:\
			{\
				\
			}\
			break\

#define END_IMPLEMENT_PACKET_FACTORY()\
		}\
		return pNewPkt;\
	}\

#define ADD_PACKET_PRODUCT(IDX,CLASS)\
		case IDX:\
		{\
			pNewPkt = new CLASS();\
		}\
		break

#define BEGINE_HANDLE_PACKET(INTERFACE_NAME)\
	MY_ASSERT(pkt.GetHeader().GetInterface() == INTERFACE_NAME);\
	switch(pkt.GetHeader().GetIndex())\
	{\
	default:{MY_ASSERT(0);}break

#define HADNLE_PACKET_(IDX,PKTTYPE)\
		case IDX:\
		{\
			HandlePacket((const PKTTYPE & )pkt);\
		}\
		break

#define END_HANDLE_PACKET()\
	}(void)(0)


}

#endif
