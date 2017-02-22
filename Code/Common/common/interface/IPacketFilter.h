#ifndef _I_PACKET_FILER_H_
#define _I_PACKET_FILER_H_


namespace Aurora
{
	class BinaryOStream;
	struct IPacketFilter
	{
	public:
		virtual ~IPacketFilter(){}

		virtual void Send(const void * pData,uint32 nDataLen,BinaryOStream & os) = 0;
		virtual void Recv(const void * pData,uint32 nDataLen,BinaryOStream & os) = 0;
	};
}

#endif
