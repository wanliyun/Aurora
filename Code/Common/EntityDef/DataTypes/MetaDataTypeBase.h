#ifndef _META_DATA_TYPE_BASE_H
#define _META_DATA_TYPE_BASE_H

#include <boost/serialization/singleton.hpp>

namespace Aurora
{
	class DataTypeBase;

	//ÔªÀàÐÍ;
	class MetaDataTypeBase
	{
	public:
		MetaDataTypeBase(const std::string & strTypeName):m_strTypeName(strTypeName){}
		virtual ~MetaDataTypeBase(){}

		const std::string & GetTypeName()const{return m_strTypeName;}

		virtual DataTypeBase * NewType(tinyxml2::XMLHandle & xml) = 0;

		static bool Init(tinyxml2::XMLHandle & xml);
		static void Fini();

		static MetaDataTypeBase * FindMetaType(const std::string & strType);

	private:
		static void AddMetaType(MetaDataTypeBase * type);
		static void AddAlias( const std::string & strType,const std::string & strOriType );
	private:
		std::string m_strTypeName;

		static std::vector<MetaDataTypeBase*> m_vMetaTypes;
		static std::map<std::string,MetaDataTypeBase*> s_mapName2MetaTypes;
	};
}

#endif
