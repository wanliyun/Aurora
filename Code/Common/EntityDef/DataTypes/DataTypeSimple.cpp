#include "pch.h"
#include "DataTypeSimple.h"
#include <boost/python/long.hpp>

namespace Aurora
{
	bool DataTypeBool::WriteToStream( BinaryOStream & oStream,bpy::object & obj,AttributeFlagT flag ) const
	{
		try
		{
			bool val = bpy::extract<bool>(obj);
			oStream<<val;
			return true;
		}
		catch(bpy::error_already_set)
		{
			PyErr_Print();
		}
		return false;
	}

// 	bool DataTypeBool::ReadFromStream( BinaryIStream & iStream ,bpy::object & obj) const
// 	{
// 		bool & val = bpy::extract<bool &>(obj);
// 		iStream>>val;
// 		return !iStream.error();
// 	}

	bpy::object DataTypeBool::NewObj( AttributeFlagT flag ) const
	{
		return bpy::object(boost::lexical_cast<bool>(m_strDftValue));
	}

	bpy::object DataTypeBool::NewObj( BinaryIStream & iStream )const
	{
		bool val;
		iStream>>val;
		return bpy::object(val);
	}
}
