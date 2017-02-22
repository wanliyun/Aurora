
#include "pch.h"
#include "Utils.h"
#include <stdlib.h>
#include <stdarg.h>
#ifdef WIN32

#else
#include <time.h>
#endif

static void SetConsoleColor(ELogType et)
{
#ifdef WIN32
	static HANDLE s_handle = INVALID_HANDLE_VALUE;
	if(s_handle == INVALID_HANDLE_VALUE)
	{
		s_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
		if(s_handle == INVALID_HANDLE_VALUE)
			return;
	}
	static WORD attr[ELogType_Count] = {
		FOREGROUND_BLUE|FOREGROUND_INTENSITY,						//Hack;
		FOREGROUND_RED|FOREGROUND_INTENSITY,						//Critical;
		FOREGROUND_RED|FOREGROUND_INTENSITY,						//Script;
		FOREGROUND_RED,												//Error;
		FOREGROUND_RED|FOREGROUND_GREEN,							//Warning;
		0x7,														//Normal;
		FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY,		//OK;
	};
	MY_ASSERT(et >= 0 && et<sizeof(attr)/sizeof(attr[0]));

	SetConsoleTextAttribute(s_handle,attr[et]);
#else
	static const char * s_colors[ELogType_Count] ={
		"\033[31m",					//Hack;
		"\033[35m",					//Critical;
		"\033[34m",					//Script;
		"\033[31m",					//Error;
		"\033[33m",					//Warning;
		"\033[37m",					//Normal;
		"\033[32m",					//OK;
	};
	MY_ASSERT(et >= 0 && et<sizeof(s_colors)/sizeof(s_colors[0]));
	printf("%s",s_colors[et]);
#endif
}

static std::string __method_name(const std::string &fsig)
{
	size_t colons = fsig.find ("::");
	size_t sbeg = fsig.substr (0, colons).rfind (" ") + 1;
	size_t send = fsig.rfind ("(") - sbeg;
	return fsig.substr (sbeg, send);
}

void LogFunc( ELogType et,const char * funcName,const char * format,... )
{
#ifdef WIN32
	printf("[%s]",funcName);
#else
	printf("[%s]",__method_name(funcName).c_str());
#endif

	SetConsoleColor(et);
	va_list args;
	const int BUFFER_LEN = 8192;
	static char  buffer[BUFFER_LEN];
	va_start( args, format );

	vsnprintf(buffer,BUFFER_LEN,format,args);
#ifdef WIN32
	printf("%s",buffer);
#else
	printf("%s",__method_name(buffer).c_str());
#endif
	va_end( args );

	SetConsoleColor(ELogType_Normal);
}

void Log4( ELogType et,const char * format,... )
{
	SetConsoleColor(et);
	va_list args;
	const int BUFFER_LEN = 4096;
	static char  buffer[BUFFER_LEN];
	va_start( args, format );

	vsnprintf(buffer,BUFFER_LEN,format,args);

	printf("%s",buffer);
	va_end(args);
	SetConsoleColor(ELogType_Normal);
}

namespace Aurora
{
	void SplitString( const std::string & str,const std::string & strSplits,std::vector<std::string> & vResult )
	{
		vResult.clear();
		std::string::size_type pos = 0;
		do{
			std::string::size_type newPos = str.find_first_of(strSplits.c_str(),pos);
			std::string split = str.substr(pos,newPos == std::string::npos ? std::string::npos : newPos - pos);
			if(!split.empty())
				vResult.push_back(split);
			pos = newPos;
		}while(pos != std::string::npos);
	}

	void SetConsoleTitle( const std::string & strTile )
	{
#ifdef WIN32
		::SetConsoleTitle(strTile.c_str());
#endif
	}

	void Sleep( uint32 nMilliSeconds )
	{
#ifdef WIN32
		::Sleep(nMilliSeconds);
#else
		timespec tm;
		tm.tv_sec = nMilliSeconds / 1000;
		tm.tv_nsec = (nMilliSeconds % 1000) * 1000 * 1000;
		nanosleep(&tm,0);
#endif
	}

}

#ifndef WIN32
int _kbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}
	return 0;
}


#endif

