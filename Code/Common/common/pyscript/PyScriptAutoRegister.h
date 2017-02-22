#ifndef _PY_SCRIPT_AUTO_REGISTER_H_
#define _PY_SCRIPT_AUTO_REGISTER_H_

struct _object;
typedef struct _object PyObject;

namespace Aurora
{
	class PyScriptAutoRegister
	{
	public:
		PyScriptAutoRegister( const char * ModuleName,PyObject* (*initfunc)(void));
	};
}

#define AUTO_REGISTER_PYTHON_MODULE(MODULE_NAME)\
	Aurora::PyScriptAutoRegister g_auto_register_##MODULE_NAME(#MODULE_NAME,PyInit_##MODULE_NAME)


#endif
