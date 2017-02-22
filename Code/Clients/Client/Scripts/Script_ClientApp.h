
#ifndef _SCRIPT_CLIENT_APP_H_
#define _SCRIPT_CLIENT_APP_H_

namespace Aurora
{
	class ClientAppWarpper
	{
	public:
		bool StartLogin(const std::string & strName,const std::string strPassword);
		void Disconnect();
		void Stop();
	};
}

#endif
