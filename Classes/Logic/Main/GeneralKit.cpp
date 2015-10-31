#include "logicHeader.h"

extern lua_State* GLuaState;
int OpenFile( const char* file )
{
  char Buf[64] = "0";
  sprintf(Buf, "GameData\\lua\\%s", file);
  return luaL_dofile(GLuaState, file);
}

//内存泄漏 待处理
char* FontToUTF8( const char* font )
{  
//   int len=MultiByteToWideChar(CP_ACP,0,font,-1,NULL,0);  
//   wchar_t* wstr=new wchar_t[len+1];  
//   memset(wstr,0,len+1);  
//   MultiByteToWideChar(CP_ACP,0,font,-1,wstr,len);  
//   len=WideCharToMultiByte(CP_UTF8,0,wstr,-1,NULL,0,NULL,NULL);  
//   char* str=new char[len+1];
//   memset(str,0,len+1);  
//   WideCharToMultiByte(CP_UTF8,0,wstr,-1,str,len,NULL,NULL);  
//   if(wstr)
//     delete[] wstr;  
//   return str;  
  return NULL;
}

bool CheckValid( int x, int y )
{
  return x>=0&&x<MatrixXNum&&y>=0&&y<MatrixYNum-1;
}

bool IsFileExist(const char* pFileName, const char* RelativePath)
{
  if( !pFileName ) 
    return false;
  string filePath;
  filePath = filePath + RelativePath + pFileName;
  string file = FileUtils::getInstance()->fullPathForFilename(filePath);
  FILE* pFile = fopen(file.c_str(), "r");
  if (!pFile)
  {
    string filePath = FileUtils::getInstance()->getWritablePath();
    filePath += pFileName;
    FILE* pFile = fopen(filePath.c_str(), "r");
  }
  return pFile?true:false;
}

void copyFile( const char* pFileName, const char* RelativePath)
{
  if (!pFileName)
    return;
  string filePath;
  filePath += RelativePath;
  filePath += pFileName;
  FileUtils* FileUtil = FileUtils::getInstance();
  string strPath = FileUtil->fullPathForFilename(filePath);
  ssize_t len = 0;
  unsigned char *data = FileUtil->getFileData(strPath.c_str(),"r",&len);
  string destPath = FileUtil->getWritablePath();
  destPath += pFileName;
  FILE *fp = fopen(destPath.c_str(),"w+");
  fwrite(data,sizeof(char),len,fp);
  fclose(fp);
  delete []data;
  data = NULL;
}

string getFilePath( const char* pFileName, const char* RelativePath )
{
  if( !pFileName ) 
    return NULL;
  string filePath;
  filePath = filePath + RelativePath + pFileName;
  string file = FileUtils::getInstance()->fullPathForFilename(filePath);
  FILE* pFile = fopen(file.c_str(), "r");
  if (!pFile)
  {
    string filePath = FileUtils::getInstance()->getWritablePath();
    filePath += pFileName;
    FILE* pFile = fopen(filePath.c_str(), "r");
    return pFile?filePath:string("");
  }
  else
  {
    return file;
  }
}
