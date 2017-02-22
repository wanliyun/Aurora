
#include "pch.h"
#include "PyScript.h"
#include <boost/python.hpp>
#include <fstream>
#include "FileSystem.h"
#include "PyImportPaths.h"
#include "Constants.h"

namespace Aurora
{
	PyScript::PyScript()
	{

	}

	PyScript::~PyScript()
	{

	}
	bool PyScript::Init( PyImportPaths & importPath,const std::string & strRunTimeModule )
	{
		//设置路径 ;
		if(1){
			PyImportPaths sysPath(PyImportPaths::DEFAULT_DELIMITER);
			sysPath.append(importPath);
			sysPath.addResPath(std::string(GetCommonScriptDir()));
			PyObject * pOriPath = PySys_GetObject("path");
			int nOriLen = PyList_GET_SIZE(pOriPath);
			PyObject * pSys = sysPath.pathAsObject();

			for(int i = 0;i<nOriLen ;++i)
			{
				PyList_Append(pSys,PyList_GET_ITEM(pOriPath,i));
			}
			int result = PySys_SetObject( "path", pSys );
			Py_DECREF( pSys );
			if (result != 0)
			{
				Log_ErrorF( "Unable to assign sys.path\n" );
				return false;
			}
		}
		

		//运行启动脚本;
		try
		{
			m_objRunTime = bpy::import(strRunTimeModule.c_str());

			m_objRunTime.attr("OnInit")();
			//m_objRunTime.attr("OnUpdate")(1.0f);
		}
		catch(bpy::error_already_set e)
		{
			PyErr_Print();
		}
	
		return true;
	}

	bool PyScript::Update( float fDeltaTime )
	{
		try
		{
			m_objRunTime.attr("OnUpdate")(fDeltaTime);
		}
		catch(...)
		{
			PyErr_Print();
		}
		return true;
	}

	void PyScript::Fini()
	{
		try
		{
			m_objRunTime.attr("OnFini")();
			m_objRunTime = bpy::object();
		}
		catch(...)
		{
			PyErr_Print();
		}
	}


	void PyScript::SetAttr( boost::python::object & ownerObj,const std::string & strAttr, boost::python::object & objVal )
	{
// 		if(bCallback)
// 		{
// 			MY_ASSERT(ownerObj.ptr());
// 			
// 			std::string strFuncName= std::string("set_") + strAttr;
// 			if(PyObject_HasAttrString(ownerObj.ptr(),strFuncName.c_str()))
// 			{
// 				PyObject* pyResult = PyObject_CallMethod(ownerObj.ptr(),const_cast<char*>(strFuncName.c_str()),const_cast<char*>("O"),objVal.ptr());
// 				if(pyResult != NULL)
// 				{
// 					Py_DECREF(pyResult);
// 				}
// 				else
// 				{
// 					PyErr_PrintEx(0);	
// 				}
// 			}
// 		}

		bpy::str attr_str(strAttr);
		PyObject_GenericSetAttr(ownerObj.ptr(), attr_str.ptr(), objVal.ptr());
	}


}
