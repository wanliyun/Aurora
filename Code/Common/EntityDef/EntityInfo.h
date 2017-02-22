#ifndef ENTITY_DEF_ENTITY_INFO_H
#define ENTITY_DEF_ENTITY_INFO_H

class TiXmlElement;
namespace Aurora
{
	class EntityAoIInfo
	{
	public:
		static const int32 DEFAULT_AOI_RADIUS = 50;
	public:
		EntityAoIInfo();
		~EntityAoIInfo(){}

		bool LoadFromXML(tinyxml2::XMLHandle ele);
		void AddToMD5(MD5 & md5)const;

		bool m_bValid;//�Ƿ���Ч,���ڼ̳�;
		int32 m_nAoIRadius;
	private:
		//��������;
		EntityAoIInfo(const EntityAoIInfo & other);
	};

	class EntityVolatileInfo
	{
	public:
		EntityVolatileInfo();
		~EntityVolatileInfo(){}

		bool LoadFromXML(tinyxml2::XMLHandle ele);
		void AddToMD5(MD5 & md5)const;
		bool m_bValid;//�Ƿ���Ч,���ڼ̳�;
		bool m_bPosX,m_bPosY,m_bPosZ;
		bool m_bYaw,m_bPitch,m_bRoll;
	private:
		//��������;
		EntityVolatileInfo(const EntityVolatileInfo & other);
	};
}

#endif
