
#include "pch.h"
#include "AccountManager.h"

namespace Aurora
{
	Account::Account( const std::string & strAccountName,AppID loginAppID,AppID clntAppId )
		:m_strAccountName(strAccountName)
		,m_state(EState_logined)
		,m_loginAppID(loginAppID)
		,m_nClntAppId(clntAppId)
		,m_nBaseAppID(BAD_APP_ID)
	{

	}


	AccountManager::AccountManager()
	{

	}

	AccountManager::~AccountManager()
	{

	}

	void AccountManager::OnTick( float fTimeDelta )
	{

	}

	Account * AccountManager::AddAccount( const std::string & strAccountName,AppID loginAppID,AppID clntAppId )
	{
		if(GetAccount(strAccountName)!=0)
			return 0;
		Account * pAccount  = new Account(strAccountName,loginAppID,clntAppId);
		m_mapAccout.insert(std::make_pair(strAccountName,pAccount));
		return pAccount;
	}

	void AccountManager::DelAccount( const std::string & strAccountName )
	{
		Account * pAccount = GetAccount(strAccountName);
		m_mapAccout.erase(strAccountName);
		SAFE_DELETE(pAccount);
	}

	Account * AccountManager::GetAccount( const std::string & strAccountName )
	{
		MapAccountT::const_iterator iter = m_mapAccout.find(strAccountName);
		if(iter == m_mapAccout.end())
			return 0;
		return iter->second;
	}
}
