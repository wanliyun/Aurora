#include "pch.h"
#include "SpaceManager.h"
#include <XMLWarpper.h>
#include <FileSystem.h>

namespace Aurora
{
	SpaceManager::SpaceManager()
	{

	}

	SpaceManager::~SpaceManager()
	{

	}

	bool SpaceManager::Init()
	{
		MY_ASSERT(GetSpaceDir());
		std::string allSpacesFileName = std::string(GetSpaceDir()) + "spaces.xml";
		tinyxml2::XMLDocument doc;
		std::string absFilePath = FileSystem::get_mutable_instance().FindFilePath(allSpacesFileName);
		if(absFilePath.empty() || tinyxml2::XML_NO_ERROR!=doc.LoadFile(absFilePath.c_str()))
		{
			Log_CriticalF("can not find file[%s]\n",allSpacesFileName.c_str());
			return false;
		}
		tinyxml2::XMLHandle hDoc(&doc);
		tinyxml2::XMLHandle hSpace  = hDoc.FirstChildElement("root").FirstChildElement("Spaces").FirstChildElement("Space");
		Log_NormalF("<<<<<<<Spaces List Begin\n");
		for(;hSpace.ToElement();hSpace = hSpace.NextSiblingElement("Space"))
		{
			XMLElementWrapper __ele(hSpace.ToElement());
			init_space(__ele);
		}
		Log_NormalF("Spaces List End>>>>>>>>>\n");
		return true;
	}

	void SpaceManager::OnTick( float fDelta )
	{

	}

	void SpaceManager::init_space( XMLElementWrapper & ele )
	{
		if(!ele)
			return;
		SpaceID sid;
		ele.GetAttribute("id",sid,BAD_SPACE_ID);
		SSpaceInfo space_info;
		ele.GetAttribute("name",space_info.m_strName);
		if(space_info.m_strName.empty())
			return;
		ele.GetAttribute("maxChannel",space_info.m_nMaxChannels);
		ele.GetAttribute("maxCell",space_info.m_nMaxCells);
		ele.GetAttribute("isMain",space_info.m_bIsMain);

		if(sid == BAD_SPACE_ID)
		{
			Log_NormalF("bad space id of [%s]\n",space_info.m_strName.c_str());
			return;
		}
		if(m_spaceInfo.m_mapSpaces.find(sid) != m_spaceInfo.m_mapSpaces.end())
		{
			Log_NormalF("duplex space id[%u] of [%s]\n",sid,space_info.m_strName.c_str());
			return;
		}
		Log_Normal("Load Space[%s] id[%u] isMain[%s] maxChannnel[%u],maxCell[%u]\n",space_info.m_strName.c_str(),sid,space_info.m_bIsMain ? "true" : "false" , space_info.m_nMaxChannels,space_info.m_nMaxCells);
		m_spaceInfo.m_mapSpaces.insert( std::make_pair(sid,space_info) );
	}
}
