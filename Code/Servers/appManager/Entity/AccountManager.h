#ifndef _ACCOUNT_MANAGER_H_
#define _ACCOUNT_MANAGER_H_

#include <map>
#include <boost/serialization/singleton.hpp>
#include "EntityDef/EntityBase.h"

namespace Aurora
{
	class Account
	{
		friend class AccountManager;
	protected:
		Account(const std::string & strAccountName,AppID loginAppID,AppID clntAppId);
		~Account(){}
	public:
		const std::string m_strAccountName;
		enum EState
		{
			EState_logined,
			EState_WaitingBaseAppCreate,
			EState_Gaming,
		};
		EState	m_state;

		const AppID	m_loginAppID;
		const AppID	m_nClntAppId;
		AppID	m_nBaseAppID;
	};

	class AccountManager : public boost::serialization::singleton<AccountManager>
	{
	public:
		AccountManager();
		~AccountManager();

		void OnTick(float fTimeDelta);

		Account * AddAccount(const std::string & strAccountName,AppID loginAppID,AppID clntAppId);
		void DelAccount(const std::string & strAccountName);

		Account * GetAccount(const std::string & strAccountName);
	private:
		typedef std::map<std::string,Account *> MapAccountT;
		MapAccountT m_mapAccout;
	};
}

#endif
