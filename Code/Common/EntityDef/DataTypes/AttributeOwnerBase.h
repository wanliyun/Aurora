#ifndef _ATTRIBUTE_OWNER_BASE_H
#define _ATTRIBUTE_OWNER_BASE_H

#include <boost/serialization/singleton.hpp>
#include <boost/core/noncopyable.hpp>

namespace Aurora
{
	struct AttributeChange : public boost::noncopyable
	{
		VAttributesT  m_vPath;
	};

	struct SingleAttributeChange : public AttributeChange
	{
		AttributeID m_index;

		bpy::object m_newVal;
	};

	struct SliceAttributeChange : public AttributeChange
	{
		AttributeID m_nStartIndex;
		AttributeID m_nEndIndex;
		std::vector<bpy::object> m_newValues;
	};

	//属性拥有者;
	class AttributeOwnerBase
	{
	public:
		AttributeOwnerBase();
		virtual ~AttributeOwnerBase(){}

		virtual void OnSingleAttributeChange(bpy::object oriVal,class DataTypeBase & dataType,SingleAttributeChange & chg);
		virtual bool OnPreSetAttr(boost::python::object obj,const std::string & attr, boost::python::object val){return false;}

	public:
		static void py_setattr(boost::python::object obj, std::string attr, boost::python::object val);

		AttributeOwnerBase *  GetOwner()const{return m_pOwner;}
		uint32 GetIndexInOwner()const{return m_nAttributeIDOfOwner;}
		void SetOwner(AttributeOwnerBase * pOwner,AttributeID nIndexOfOwner){m_pOwner = pOwner;m_nAttributeIDOfOwner = nIndexOfOwner;}
	private:
		AttributeOwnerBase * m_pOwner;		//不拥有;
		AttributeID m_nAttributeIDOfOwner;
	};
}

#endif
