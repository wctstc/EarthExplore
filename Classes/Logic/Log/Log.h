#pragma once
#include <string>

#define check(expr) if (!expr){CLog::GetInstance()->LogOut(__FILE__, __FUNCTION__, __LINE__, #expr);assert(expr);}

class CLog
{
public:
  static CLog* GetInstance();
  static CLog* _Log;
private:
  CLog();
public:
  void MakeDir();
  void LogOut(const char* file, const char *function, int line, const char *format, ... );
private:
  std::string _file;
};