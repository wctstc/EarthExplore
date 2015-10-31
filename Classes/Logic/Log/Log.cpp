#include "logicHeader.h"

CLog* CLog::_Log = NULL;

CLog* CLog::GetInstance()
{
  if (!_Log)
    _Log = new CLog();
  return _Log;
}

CLog::CLog()
{
  MakeDir();
}

void CLog::MakeDir()
{
  const char* Dir = "sdcard/EarthExplore/";
  const char* file = "EarthExplore.txt";
  string fileDir;
  fileDir = fileDir+Dir+file;
  FileUtils* File = FileUtils::getInstance();
  bool create = File->createDirectory(Dir);
  _file = fileDir.c_str();
  FILE* fp = fopen(fileDir.c_str(), "w");
  if (fp)
    fclose(fp);
}

void CLog::LogOut(const char* file, const char *function, int line, const char *format, ... )
{
  //将可变参数格式化到buffer
  char buffer[2048];
  va_list list;
  va_start( list, format );
  vsnprintf(buffer, sizeof(buffer)-1, format, list );
  va_end( list );
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
  //获取时间
  char time_buffer[64] = {0};
  time_t curtime=time(0); 
  tm tim =*localtime(&curtime); 
  int day,mon,year; 
  day=tim.tm_mday;
  mon=tim.tm_mon;
  year=tim.tm_year;
  sprintf(time_buffer, "%d-%d-%d %d:%d:%d  ", 
    tim.tm_year+1900, tim.tm_mon+1, tim.tm_mday, tim.tm_hour, tim.tm_min, tim.tm_sec);

  //日志输出
  ofstream fout( _file.c_str(), ios::app );
  if( fout.is_open() )
  {
    fout << time_buffer
      << " " << file
      << " " << function
      << " " << line
      << " " << buffer << endl;
    fout.close();
  }
  //控制台输出
  cout<< time_buffer
    << " " << _file.c_str()
    << " " << function
    << " " << line
    << " " << buffer << endl;
}

