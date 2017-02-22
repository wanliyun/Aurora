
#include "pch.h"
#include "PyScriptAutoRegister.h"

namespace Aurora
{
	PyScriptAutoRegister::PyScriptAutoRegister( const char * ModuleName,PyObject* (*initfunc)(void) )
	{
		PyImport_AppendInittab(ModuleName,initfunc);
	}
}

