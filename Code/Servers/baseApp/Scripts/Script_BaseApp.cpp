
#include "pch.h"
#include "BaseApp.h"
#include "Script_BaseApp.h"

namespace Aurora
{
	void BaseAppWarpper::Stop()
	{
		BaseApp::get_mutable_instance().ShutDown();
	}
}




