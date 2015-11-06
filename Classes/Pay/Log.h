#ifndef __LOG_MANGAGE_H__
#define __LOG_MANGAGE_H__

/*

全局日志使用：
G_Log("hello world! %d", 10 );

成员日志使用：
CLog log( "person.log" );
log.M_Log( "hello world! %d", 10 );

**不同app需修改DIR和m_instance的初始化值**

*/


#include <string>
#include <stdarg.h>

using namespace std;

//全局日志，共享一个日志文件
#define G_Log( format, ... ) CLog::GetInstance()->LogOut( __DATE__, __TIME__, __FILE__, __FUNCTION__ , __LINE__, format, ##__VA_ARGS__ )
//成员日志，对象调用
#define M_Log( format, ... ) LogOut( __DATE__, __TIME__, __FILE__, __FUNCTION__ , __LINE__, format, ##__VA_ARGS__ )


class CLog
{
public:
	CLog( string file_name );

	static CLog *GetInstance();
public:
	void LogOut( const char *date, const char *time, const char *src_file, const char *function, int line, const char *format, ... );
private:
	string m_file_name;
	static CLog *m_instance;
};


#endif
