#include "pch.h"
#include "EntityInfo.h"
namespace Aurora
{

	EntityAoIInfo::EntityAoIInfo()
		:m_bValid(false)
		,m_nAoIRadius(DEFAULT_AOI_RADIUS)
	{

	}


	bool EntityAoIInfo::LoadFromXML( tinyxml2::XMLHandle ele )
	{
		if(!ele.ToElement())
			return false;

		XMLElementWrapper warpper(ele.ToElement());
		m_bValid = warpper.GetAttribute("radius",m_nAoIRadius,DEFAULT_AOI_RADIUS);

		return m_bValid;
	}

	void EntityAoIInfo::AddToMD5( MD5 & md5 ) const
	{
		md5.Append( &m_nAoIRadius, sizeof(m_nAoIRadius) );
	}

	EntityVolatileInfo::EntityVolatileInfo()
		:m_bValid(false)
		,m_bPosX(true)
		,m_bPosY(true)
		,m_bPosZ(true)
		,m_bYaw(true)
		,m_bPitch(false)
		,m_bRoll(false)
	{

	}

	bool EntityVolatileInfo::LoadFromXML( tinyxml2::XMLHandle ele )
	{
		if(ele.FirstChildElement("pos").ToElement())
		{
			XMLElementWrapper warpper(ele.FirstChildElement("pos").ToElement());
			m_bValid |= warpper.GetAttribute("x",m_bPosX,true);
			m_bValid |= warpper.GetAttribute("y",m_bPosY,true);
			m_bValid |= warpper.GetAttribute("z",m_bPosZ,true);
		}

		if(ele.FirstChildElement("rot").ToElement())
		{
			XMLElementWrapper warpper(ele.FirstChildElement("pos").ToElement());
			m_bValid |= warpper.GetAttribute("y",m_bYaw,true);
			m_bValid |= warpper.GetAttribute("p",m_bPitch,false);
			m_bValid |= warpper.GetAttribute("r",m_bRoll,false);
		}

		return m_bValid;
	}

	void EntityVolatileInfo::AddToMD5( MD5 & md5 ) const
	{
		md5.Append(m_bPosX ? "1" :"0",1);
		md5.Append(m_bPosY ? "1" :"0",1);
		md5.Append(m_bPosZ ? "1" :"0",1);
		md5.Append(m_bYaw  ? "1" :"0",1);
		md5.Append(m_bPitch? "1" :"0",1);
		md5.Append(m_bRoll ? "1" :"0",1);
	}
}

