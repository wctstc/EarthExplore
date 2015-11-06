#include "Log.h"
#include "CCFileUtils.h"

#include <fstream>
#include <time.h>
#include <iostream>

#ifdef WIN32
#include <direct.h>

#define MKDIR(dir) _mkdir( dir )
#else
#include <sys/stat.h>
#include <sys/types.h>
#define MKDIR(dir) mkdir( dir, S_IRWXU|S_IRWXG|S_IRWXO )
#endif


static const string DIR = "/mnt/sdcard/EarthExplore/";

CLog * CLog::m_instance = NULL;


CLog::CLog( string file_name )
{
	cocos2d::FileUtils::getInstance()->createDirectory(DIR);
	m_file_name = DIR + file_name;
}
	
CLog * CLog::GetInstance()
{
	if( m_instance == NULL )
		m_instance = new CLog("EarthExplore.log");
	return m_instance;
}

void CLog::LogOut( const char *date, const char *time, const char *src_file, const char *function, int line, const char *format, ... )
{
	//截取源文件名
	string src_file_name;
	src_file_name = src_file;
	src_file_name = src_file_name.substr( src_file_name.find_last_of('\\')+1 );


	char buffer[2048];
	va_list list;
	//读取可变参数
	va_start( list, format );
	vsnprintf(buffer, sizeof(buffer) - 1, format, list );
	va_end( list );
		
	ofstream fout( m_file_name.c_str(), ios::app );
	if( fout.is_open() )
	{
		fout << date
			<< " " << time
			<< " " << src_file_name.c_str()
			<< " " << function
			<< " " << line
			<< " " << buffer << endl;
		fout.close();
	}
}
