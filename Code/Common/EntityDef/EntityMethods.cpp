
#include "pch.h"
#include "EntityMethods.h"
#include "DataTypes/DataTypeBase.h"

namespace Aurora
{
	EntityMethod::EntityMethod( )
		:DataTypeBase(0)
		,m_bIsExposed(false)
	{

	}

	EntityMethod::~EntityMethod()
	{
		
	}

	bool EntityMethod::LoadFromXML( tinyxml2::XMLElement  & xmlEle )
	{
		//方法名称;
		{
			m_strRefName  = std::string(xmlEle.Value());
			XMLElementWrapper eleWarpper(&xmlEle);
			eleWarpper.GetAttribute("exposed",m_bIsExposed,true);
		}
		
		//参数列表;
		for(tinyxml2::XMLElement * pArg = xmlEle.FirstChildElement();pArg;pArg = pArg->NextSiblingElement())
		{
			const std::string strArgName(pArg->Name());
			if(m_name2Id.find(strArgName) != m_name2Id.end())
			{
				Log_ErrorF("func [%s] duplex arg [%s]\n",GetName().c_str(),strArgName.c_str());
				return false;
			}

			const char * pSzType = pArg->Attribute("type");
			if(!pSzType)
			{
				Log_ErrorF("func [%s] arg [%s] has no argType\n",GetName().c_str(),strArgName.c_str());
				return false;
			}
			
			boost::shared_ptr<DataTypeBase> pArgType = DataTypeBase::GetType(pSzType);
			if(!pArgType)
			{
				pArgType = DataTypeBase::CreateType(pSzType,tinyxml2::XMLHandle(pArg));
				if(!pArgType)
				{
					Log_ErrorF("func [%s] arg [%s] bad arg type[%s]\n",GetName().c_str(),strArgName.c_str(),pSzType);
					return false;
				}
			}

			m_vArgs.push_back(pArgType);
			m_name2Id.insert( std::make_pair(std::string(pArg->Value()),AttributeID(m_vArgs.size()-1)));
		}
		return true;
	}

	void EntityMethod::AddToMD5( MD5 & md5 ) const
	{
		md5.Append(GetName().c_str(),int(GetName().size()));
		for(VDataTypeT::const_iterator iter = m_vArgs.begin();iter != m_vArgs.end();++iter)
		{
			(*iter)->AddToMD5(md5);
		}
	}

	bool EntityMethod::WriteToStream( BinaryOStream & oStream,bpy::object & obj,AttributeFlagT flag ) const
	{
		PyObject * pTuple = obj.ptr();
		if(!pTuple)
		{
			Log_CriticalF("ptuple is null\n");
			return false;
		}
		if(!PyTuple_Check(pTuple))
		{
			Log_CriticalF("pTuple is not a tuple\n");
			return false;
		}
		if(PyTuple_GET_SIZE(pTuple) != m_vArgs.size())
		{
			Log_ScriptF("func[%s] require [%d] args while [%d] args\n",GetName().c_str(),m_vArgs.size(),PyTuple_GET_SIZE(pTuple));
			return false;
		}
		for(size_t i = 0;i<m_vArgs.size();++i)
		{
			PyObject * pArgObj = PyTuple_GET_ITEM(pTuple,i);
			MY_ASSERT(pArgObj && m_vArgs[i]);

			bpy::object obj(bpy::handle<>(bpy::borrowed(pArgObj)));
			if(!m_vArgs[i]->WriteToStream(oStream,obj,EFlag_All))
				return false;
		}
		return true;
	}
	bpy::object EntityMethod::NewObj( AttributeFlagT flag ) const
	{
		MY_ASSERT(false);
		return bpy::object();
	}

	bpy::object EntityMethod::NewObj( BinaryIStream & iStream ) const
	{
		PyObject * pRetTuple = PyTuple_New(m_vArgs.size());
		if(!pRetTuple)
		{
			Log_CriticalF("PyTuple_new() failed\n");
			return bpy::object();
		}

		for(size_t i = 0;i<m_vArgs.size();++i)
		{
			MY_ASSERT( m_vArgs[i]);
			bpy::object obj = m_vArgs[i]->NewObj(iStream);
			PyObject * pArg = obj.ptr();
			Py_IncRef(pArg);
			PyTuple_SET_ITEM(pRetTuple,i,pArg);
		}
		return bpy::object(bpy::handle<>(pRetTuple));
	}


	EntityMethodGroup::EntityMethodGroup()
	{

	}

	EntityMethodGroup::~EntityMethodGroup()
	{
	}

	bool EntityMethodGroup::HasMethod( const std::string & name ) const
	{
		return m_mapMethods.find(name)!=m_mapMethods.end();
	}

	bool EntityMethodGroup::CopyMethods( EntityMethodGroup & target ) const
	{
		for(VEntityMethodT::const_iterator iter= m_vMethods.begin();iter != m_vMethods.end();++iter)
		{
			if(target.HasMethod((*iter)->GetName()))
			{
				Log_ErrorF("method [%s] already exists in target\n",(*iter)->GetName().c_str());
				continue;
			}

			target.m_vMethods.push_back(*iter);
		}
		target.rebuildMap();
		return true;
	}

	bool EntityMethodGroup::LoadFromXML( tinyxml2::XMLHandle  handle )
	{
		tinyxml2::XMLElement * pEle = handle.FirstChild().ToElement();
		for(;pEle;pEle=pEle->NextSiblingElement())
		{
			boost::shared_ptr<EntityMethod> pMethod(new EntityMethod());
			if(!pMethod->LoadFromXML(*pEle))
			{
				Log_ErrorF("bad method=[%s]\n",pMethod->GetName().c_str());
				continue;
			}

			if(HasMethod(pMethod->GetName()))
			{
				Log_ErrorF("duplex method=[%s]\n",pMethod->GetName().c_str());
				continue;
			}
			
			m_vMethods.push_back(pMethod);
		}
		rebuildMap();
		return true;
	}

	void EntityMethodGroup::AddToMD5( MD5 & md5 ) const
	{
		for(VEntityMethodT::const_iterator iter = m_vMethods.begin();iter != m_vMethods.end();++iter)
			(*iter)->AddToMD5(md5);
	}

	boost::shared_ptr<EntityMethod> EntityMethodGroup::GetMethod( const std::string & name,MethodID & idx ) const
	{
		MapName2IDT::const_iterator iter= m_mapMethods.find(name);
		if(iter == m_mapMethods.end())
			return 0;
		
		idx = iter->second;
		MY_ASSERT(idx < m_vMethods.size() );
		MY_ASSERT(m_vMethods[idx]);
		return m_vMethods[idx];
	}

	boost::shared_ptr<EntityMethod> EntityMethodGroup::GetMethod( MethodID idx ) const
	{
		if(idx >= m_vMethods.size())
			return 0;
		return m_vMethods[idx];
	}

	void EntityMethodGroup::rebuildMap()
	{
		if(m_vMethods.size() >= MethodID(-1))
		{
			Log_ScriptF("method size[%u] not support,max[%u]\n",m_vMethods.size(),MethodID(-1));
			return;
		}

		for(size_t methodId = 0;methodId<m_vMethods.size();++methodId)
		{
			if(m_mapMethods.find(m_vMethods[methodId]->GetName()) != m_mapMethods.end())
			{
				Log_ScriptF("duplex method[%s]. use new\n",m_vMethods[methodId]->GetName().c_str());
				m_mapMethods[m_vMethods[methodId]->GetName()] = MethodID(methodId);
			}
			else
			{
				m_mapMethods.insert( std::make_pair(m_vMethods[methodId]->GetName(),MethodID(methodId)) );
			}
		}
	}
}


