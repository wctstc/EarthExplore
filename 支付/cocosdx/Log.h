#ifndef __LOG_MANGAGE_H__
#define __LOG_MANGAGE_H__

/*

ȫ����־ʹ�ã�
G_Log("hello world! %d", 10 );

��Ա��־ʹ�ã�
CLog log( "person.log" );
log.M_Log( "hello world! %d", 10 );

**��ͬapp���޸�DIR��m_instance�ĳ�ʼ��ֵ**

*/


#include <string>
#include <stdarg.h>

using namespace std;

//ȫ����־������һ����־�ļ�
#define G_Log( format, ... ) CLog::GetInstance()->LogOut( __DATE__, __TIME__, __FILE__, __FUNCTION__ , __LINE__, format, ##__VA_ARGS__ )
//��Ա��־���������
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
