
#include "pch.h"
#include "CellApp.h"
#include "Script_CellApp.h"

namespace Aurora
{
	void CellAppWarpper::Stop()
	{
		CellApp::get_mutable_instance().ShutDown();
	}
}




