#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>
#include "Types.h"


	//void Log(const char * format,...);

	//void Log2(const char * prefix,const char * format,...);

	enum ELogType
	{
		ELogType_Hack		,
		ELogType_Critical	,
		ELogType_Script		,
		ELogType_Error		,
		ELogType_Warning	,
		ELogType_Normal		,
		ELogType_OK,
		ELogType_Count
	};
	
	void LogFunc(ELogType et,const char * funcName,const char * format,...);
	void Log4(ELogType et,const char * format,...);

#define Log_Hack(FORMAT, ...)		Log4(ELogType_Hack		,FORMAT,##__VA_ARGS__)
#define Log_Critical(FORMAT, ...)	Log4(ELogType_Critical	,FORMAT,##__VA_ARGS__)
#define Log_Script(FORMAT, ...)		Log4(ELogType_Script	,FORMAT,##__VA_ARGS__)
#define Log_Error(FORMAT, ...)		Log4(ELogType_Error		,FORMAT,##__VA_ARGS__)
#define Log_Warning(FORMAT, ...)	Log4(ELogType_Warning	,FORMAT,##__VA_ARGS__)
#define Log_Normal(FORMAT, ...)		Log4(ELogType_Normal	,FORMAT,##__VA_ARGS__)
#define Log_OK(FORMAT, ...)			Log4(ELogType_OK		,FORMAT,##__VA_ARGS__)

#ifdef WIN32
	#define __MY_FUNC_ __FUNCTION__
#else
	#define __MY_FUNC_ __PRETTY_FUNCTION__
#endif

#define Log_HackF(FORMAT, ...)		LogFunc(ELogType_Hack		,__MY_FUNC_,FORMAT,##__VA_ARGS__)
#define Log_CriticalF(FORMAT, ...)	LogFunc(ELogType_Critical	,__MY_FUNC_,FORMAT,##__VA_ARGS__)
#define Log_ScriptF(FORMAT, ...)	LogFunc(ELogType_Script		,__MY_FUNC_,FORMAT,##__VA_ARGS__)
#define Log_ErrorF(FORMAT, ...)		LogFunc(ELogType_Error		,__MY_FUNC_,FORMAT,##__VA_ARGS__)
#define Log_WarningF(FORMAT, ...)	LogFunc(ELogType_Warning	,__MY_FUNC_,FORMAT,##__VA_ARGS__)
#define Log_NormalF(FORMAT, ...)	LogFunc(ELogType_Normal		,__MY_FUNC_,FORMAT,##__VA_ARGS__)
#define Log_OKF(FORMAT, ...)		LogFunc(ELogType_OK			,__MY_FUNC_,FORMAT,##__VA_ARGS__)


#define LogF(FORMAT,...)			LogFunc(ELogType_Normal		,__MY_FUNC_,FORMAT,##__VA_ARGS__)
#define LogFX() LogF("\n")

#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(x)	if( (x)!=NULL ) { free(x); (x)=NULL; }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)	if( (x)!=NULL ) { (x)->Release(); (x)=NULL; }
#endif

namespace Aurora
{
	void  SplitString(const std::string & str,const std::string & strSplits,std::vector<std::string> & vResult);

	//设置控制台颜色;
	void SetConsoleTitle(const std::string & strTile);

	void Sleep(uint32 nMilliSeconds);
}

#ifndef WIN32
int _kbhit(void);
#endif




#endif //_UTILS_H_
