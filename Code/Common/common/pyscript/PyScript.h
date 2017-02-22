#ifndef _PY_SCRIPT_H_
#define _PY_SCRIPT_H_

#include "PyImportPaths.h"
#include <boost/serialization/singleton.hpp>

namespace Aurora
{
	class PyScript : public boost::serialization::singleton<PyScript>
	{
	public:
		PyScript();
		~PyScript();
		bool Init(PyImportPaths & importPath,const std::string & strRunTimeModule);
		bool Update(float fDeltaTime);
		void Fini();

		static void SetAttr(boost::python::object & ownerObj,const std::string & strAttr, boost::python::object & objVal);
	private:
		boost::python::object m_objRunTime;
	};
}

#endif
