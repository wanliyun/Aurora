
#include "pch.h"
#include "ClientApp.h"
#include "Script_ClientApp.h"

namespace Aurora
{

	bool ClientAppWarpper::StartLogin( const std::string & strName,const std::string strPassword )
	{
		return ClientApp::get_mutable_instance().StartLogin(strName,strPassword);
	}

	void ClientAppWarpper::Disconnect()
	{
		ClientApp::get_mutable_instance().Disconnect();
	}

	void ClientAppWarpper::Stop()
	{
		ClientApp::get_mutable_instance().Stop();
	}

}




