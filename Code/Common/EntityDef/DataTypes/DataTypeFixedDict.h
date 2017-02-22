#ifndef _DATA_TYPE_FIXED_DICT_H
#define _DATA_TYPE_FIXED_DICT_H

#include "DataTypeBase.h"
#include "binary_stream.h"
#include <boost/lexical_cast.hpp>
#include "PyTypeLongInteger.h"
#include "MetaDataTypeBase.h"
#include "AttributeOwnerBase.h"
#include <iostream>

namespace Aurora
{
	class DataTypeFixDict;
	class DataTypeFixedDictMeta : public MetaDataTypeBase
	{
	public:
		DataTypeFixedDictMeta(const std::string & strMetaTypeName):MetaDataTypeBase(strMetaTypeName){}

		static bool BuildDictType(DataTypeFixDict & dictType,tinyxml2::XMLHandle & xml);
	private:
		//override MetaDataTypeBase;
		virtual DataTypeBase * NewType( tinyxml2::XMLHandle & xml );
		//~override MetaDataTypeBase;
	};

	class DataTypeFixDict : public DataTypeBase
	{
	public:
		DataTypeFixDict(MetaDataTypeBase * pMetaType):DataTypeBase(pMetaType){}

		virtual bpy::object NewObj(AttributeFlagT flag)const;
		virtual bpy::object NewObj(BinaryIStream & iStream)const;

		virtual bool WriteToStream(BinaryOStream & oStream,bpy::object & obj,AttributeFlagT flag)const;

		virtual AttributeOwnerBase * AsAttributeOwner(bpy::object obj);

	public:
		void AppendChildType(const std::string & strName,boost::shared_ptr<DataTypeBase> pChildType );

		virtual AttributeID GetChildCount()const{return AttributeID(m_vChilds.size());}
		virtual DataTypeBasePtr GetChildTypeByID(AttributeID nIndex)const;

		DataTypeBasePtr GetChildTypeByName(const std::string & strName)const;
		bool GetChildIdByName(const std::string & strName, AttributeID & id)const;
		bool GetChildNameByID(AttributeID nIndex,std::string & strName)const;

		void Inhert(DataTypeFixDict & childType);
	private:
		virtual void OnAttach(bpy::object obj,AttributeOwnerBase * pOwner,uint32 indexInOwner);
		virtual void OnDetach(bpy::object obj);

	protected:
		VDataTypeT m_vChilds;
		Name2IDMapT m_mapName2ID;
		ID2NameMapT m_mapID2Name;
	};
	typedef boost::shared_ptr<DataTypeFixDict> DataTypeFixDictPtr;


	class DataTypeFixDict_PyObject : public AttributeOwnerBase
	{
	public:
		DataTypeFixDict_PyObject(const DataTypeFixDict * pDataType);
		
		std::string ToString()const;

		bool Create(AttributeFlagT flag);
		bool CreateFromStream(BinaryIStream & is);

		bool WriteToStream(BinaryOStream & oStream,bpy::object & obj,AttributeFlagT flag);

		bool HasKey(const std::string & strKey)const;

		bpy::object GetKeys();
		bpy::object GetValues();
		bpy::object GetItems();

		static AttributeID py_len(boost::python::object obj);

		static bpy::object py_getattr(boost::python::object obj, std::string attr);

		static bool py_contains(boost::python::object obj, std::string attr);
	private:
		virtual bool OnPreSetAttr(bpy::object obj,const std::string & attr, bpy::object val);

	private:
		const DataTypeFixDict & m_dataType;
		typedef std::map<std::string,bpy::object> MapName2ChildObjT;
		MapName2ChildObjT m_mapName2ChildObjs;
	};
	typedef boost::shared_ptr<DataTypeFixDict_PyObject> DataTypeFixDict_PyObjectPtr;
}

#endif
