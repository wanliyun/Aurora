#include "pch.h"
#include "AttributeOwnerBase.h"

namespace Aurora
{
	AttributeOwnerBase::AttributeOwnerBase()
		:m_pOwner(0)
		,m_nAttributeIDOfOwner(BAD_ATTRIBUTE_ID)
	{

	}

	void AttributeOwnerBase::OnSingleAttributeChange( bpy::object oriVal,DataTypeBase & dataType,SingleAttributeChange & chg )
	{
		chg.m_vPath.push_back( GetIndexInOwner() );

		if(GetOwner())
			GetOwner()->OnSingleAttributeChange(oriVal,dataType,chg);
	}

	void AttributeOwnerBase::py_setattr( boost::python::object obj, std::string attr, boost::python::object val )
	{
		AttributeOwnerBase & thisObj = bpy::extract<AttributeOwnerBase &>(obj);
		if(thisObj.OnPreSetAttr(obj,attr,val))
			return;

		bpy::str attr_str(attr);
		PyObject_GenericSetAttr(obj.ptr(), attr_str.ptr(), val.ptr());
	}
}
