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

		bool m_bValid;//是否有效,用于继承;
		int32 m_nAoIRadius;
	private:
		//仅仅声明;
		EntityAoIInfo(const EntityAoIInfo & other);
	};

	class EntityVolatileInfo
	{
	public:
		EntityVolatileInfo();
		~EntityVolatileInfo(){}

		bool LoadFromXML(tinyxml2::XMLHandle ele);
		void AddToMD5(MD5 & md5)const;
		bool m_bValid;//是否有效,用于继承;
		bool m_bPosX,m_bPosY,m_bPosZ;
		bool m_bYaw,m_bPitch,m_bRoll;
	private:
		//仅仅声明;
		EntityVolatileInfo(const EntityVolatileInfo & other);
	};
}

#endif
