#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "logicheader.h"

const char* TXT_PATH = "GameData/txt/";
const char* LUA_PATH = "GameData/lua/";

#define _FILESYSTEM

extern lua_State* GLuaState;
/************************************************************************/
/*                                                                      */
/************************************************************************/
GParse::GParse(char* Str)
{
  _Index = 0;
  char* tok = "\t";
  char* ptemp = strtok(Str, tok);
  while(ptemp)
  {
    _StrtokStr.push_back(ptemp);
    ptemp = strtok(NULL, tok);  
  }
}

int GParse::getInt()
{
  int ret = atoi(_StrtokStr[_Index++].c_str());
  return ret;
}

std::string GParse::getString()
{
  string ret = _StrtokStr[_Index++];
  return ret;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

bool CItemMgr::LoadConfig()
{ 
  char* fileName = "ItemProperty.txt";
  if (!IsFileExist(fileName, TXT_PATH))
    copyFile(fileName, TXT_PATH);
  string fullFilePath = getFilePath(fileName, TXT_PATH);
  int fileLen = strlen(fullFilePath.c_str());
  if (!fileLen)
    return false;
  char buffer[256];    
  ifstream in(fullFilePath.c_str());
  //ifstream in("GameData\\txt\\ItemProperty.txt");  
  if (! in.is_open())  
  { 
    cout << "Error opening file ItemProperty"; 
    return false; 
  }
  int Line = 0;
  while (EOF != in.peek() )  
  { 
    in.getline (buffer,256);
    if (Line++<1)
      continue;
    GParse GP(buffer);
    int Id = GP.getInt();
    ItemProperty Property;
    Property.Id = Id;
    Property.Name = GP.getString();
    Property.Describe = GP.getString();
    Property.Pic = GP.getString();
    Property.Type = GP.getInt();
    Property.AutoDown = GP.getInt()>0;
    Property.ElimitNum = GP.getInt();
    Property.RangeType = GP.getInt();
    Property.DetailType = GP.getInt();
    Property.Life = GP.getInt();
    Items.insert(pair<int, ItemProperty>(Id, Property));
  }
  return true;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/


bool CTestLuaMgr::LoadConfig()
{
  if (luaL_dofile(GLuaState, "GameData\\lua\\stage.lua"))
    return false;
  CLuaObject LuaObject = CLuaObject("stage", GLuaState);
  BEGIN_LUA(LuaObject)
  {
    int Level = LuaObject.getInt("Level");
    int Type = LuaObject.getInt("Type");
  }
  END_LUA(LuaObject)
  return true;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

CDataManager* DataMgr = new CDataManager;
CDataManager* CDataManager::GetInstance()
{
//   if (!m_Instance)
//     m_Instance = new CDataManager;
//   return m_Instance;
  return NULL;
}

bool CDataManager::LoadConfig()
{
  if (!GameString.LoadConfig())
    return false;
  //return true;
  if (!BlockMatrix.LoadConfig())
    return false;
  if (!ItemMgr.LoadConfig())
    return false;
  if (!GlobleData.LoadConfig())
    return false;
  if (!EliminateScore.LoadConfig())
    return false;
  if (!DefaultItem.LoadConfig())
    return false;
  if (!ProperSet.LoadConfig())
    return false;
  if (!Shop.LoadConfig())
    return false;
  return true;
}

ItemProperty* CDataManager::GetItemProperty( int Id )
{
   map<int, ItemProperty>::iterator  iter = ItemMgr.Items.find(Id);
   if (iter != ItemMgr.Items.end())
     return &iter->second;
   return NULL;
}

const char* CDataManager::GetGameString( int Id )
{
  return GameString.GameStrings[Id-1].c_str();
}

CDataManager::CDataManager()
{

}

/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CEliminateScore::LoadConfig()
{
#ifdef _FILESYSTEM
  char* fileName = "Score.txt";
  if (!IsFileExist(fileName, TXT_PATH))
    copyFile(fileName, TXT_PATH);
  string fullFilePath = getFilePath(fileName, TXT_PATH);
  int fileLen = strlen(fullFilePath.c_str());
  if (!fileLen)
    return false;
  char buffer[256];  
  ifstream in(fullFilePath);  
  if (! in.is_open())  
  { 
    cout << "Error opening file Score"; 
    return false; 
  }
  int Line = 0;
  while (EOF != in.peek() )  
  { 
    in.getline (buffer,256);
    GParse GP(buffer);
    ScoreTable ScoreT;
    ScoreT.Num = GP.getInt();
    ScoreT.Score = GP.getInt();
    Scores.push_back(ScoreT);
  }  
  return true;
#endif;
  /*
  char* fileName = "Score.lua";
  if (!IsFileExist(fileName, LUA_PATH))
    copyFile(fileName, LUA_PATH);
  string fullFilePath = getFilePath(fileName, LUA_PATH);
  int fileLen = strlen(fullFilePath.c_str());
  if (!fileLen)
    return false;
  if (luaL_dofile(GLuaState, fullFilePath.c_str()))
  {
    CCLOG("Score.lua file error!");
    return false;
  }
  CLuaObject LuaObject = CLuaObject("ScoreData", GLuaState);
  BEGIN_LUA(LuaObject)
  {
    ScoreTable ScoreT;
    ScoreT.Num = LuaObject.getInt("Num");
    ScoreT.Score = LuaObject.getInt("Score");
    Scores.push_back(ScoreT);
  }
  END_LUA(LuaObject)*/
  return true;
}

int CEliminateScore::GetSocre( int Num )
{
  if (!Num)
    return 0;
  int EliminateNum = Scores.size();
  for (int i = 0; i < EliminateNum; i++)
  {
    if (Scores[i].Num == Num)
      return Scores[i].Score;
  }
  return EliminateNum?Scores[EliminateNum-1].Score:0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
bool CDefaultItem::LoadConfig()
{  
#ifdef _FILESYSTEM
  char* fileName = "defaultItem.txt";
  if (!IsFileExist(fileName, TXT_PATH))
    copyFile(fileName, TXT_PATH);
  string fullFilePath = getFilePath(fileName, TXT_PATH);
  int fileLen = strlen(fullFilePath.c_str());
  if (!fileLen)
    return false;
  char buffer[256];  
  ifstream in(fullFilePath);  
  if (! in.is_open())  
  { 
    cout << "Error opening file defaultItem"; 
    return false; 
  }
  int Line = 0;
  while (EOF != in.peek() )  
  { 
    in.getline (buffer,256);
    GParse GP(buffer);
    ItemRecord Item;
    Item.ItemId = GP.getInt();
    Item.ItemNum = GP.getInt();
    GSelfCtl->PushItem(Item);
  }
  return true;
#endif;
  /*
  char* fileName = "defaultItem.lua";
  if (!IsFileExist(fileName, LUA_PATH))
    copyFile(fileName, LUA_PATH);
  string fullFilePath = getFilePath(fileName, LUA_PATH);
  int fileLen = strlen(fullFilePath.c_str());
  if (!fileLen)
    return false;
  if (luaL_dofile(GLuaState, fullFilePath.c_str()))
  {
    CCLOG("defaultItem.lua file error!");
    return false;
  }
  CLuaObject LuaObject = CLuaObject("defaultItem", GLuaState);
  BEGIN_LUA(LuaObject)
  {
    ItemRecord Item;
    Item.ItemId = LuaObject.getInt("Item");
    Item.ItemNum = LuaObject.getInt("Num");
    GSelfCtl->PushItem(Item);
  }
  END_LUA(LuaObject)*/
  return true;
}

/************************************************************************/
/* GameString                                                           */
/************************************************************************/
bool CGameString::LoadConfig()
{
  char* fileName = "GameString.txt";
  if (!IsFileExist(fileName, TXT_PATH))
    copyFile(fileName, TXT_PATH);
  string fullFilePath = getFilePath(fileName, TXT_PATH);
  int fileLen = strlen(fullFilePath.c_str());
  if (!fileLen)
    return false;
  char buffer[256];  
  ifstream in(fullFilePath);  
  if (! in.is_open())  
  { 
    cout << "Error opening file ItemProperty"; 
    return false; 
  }
  int Line = 0;
  while (EOF != in.peek() )  
  { 
    in.getline (buffer,256);
    GParse GP(buffer);
    GP.getInt();
    string Descibe = GP.getString();
    GameStrings.push_back(/*FontToUTF8*/(Descibe.c_str()));
  }  
  return true;
}

const string& CGameString::getString( int idx )
{
  return GameStrings[idx-1];
}

/************************************************************************/
/* 全局数据                                                             */
/************************************************************************/
bool CGlobleData::LoadConfig()
{
#ifdef _FILESYSTEM
  char* fileName = "GlobleData.txt";
  if (!IsFileExist(fileName, TXT_PATH))
    copyFile(fileName, TXT_PATH);
  string fullFilePath = getFilePath(fileName, TXT_PATH);
  int fileLen = strlen(fullFilePath.c_str());
  if (!fileLen)
    return false;
  char buffer[256];  
  ifstream in(fullFilePath);  
  if (! in.is_open())  
  { 
    cout << "Error opening file GlobleData"; 
    return false; 
  }
  while (EOF != in.peek() )  
  { 
    in.getline (buffer,256);
    GParse GP(buffer);
    string Key = GP.getString();
    int Value = GP.getInt();
    GlobleValue.insert(pair<string, int>(Key, Value));
  }
  GSelfCtl->_PlayerData._BulletNum = getValue("BulletNum");
  return true;
#endif;
  /*
  char* fileName = "GlobleData.lua";
  if (!IsFileExist(fileName, LUA_PATH))
    copyFile(fileName, LUA_PATH);
  string fullFilePath = getFilePath(fileName, LUA_PATH);
  int fileLen = strlen(fullFilePath.c_str());
  if (!fileLen)
    return false;
  if (luaL_dofile(GLuaState, fullFilePath.c_str()))
  {
    CCLOG("GlobleData.lua file error!");
    return false;
  }
  CLuaObject LuaObject = CLuaObject(GLuaState);
  int Num = LuaObject.getgInt("BlockToleNum");
  GSelfCtl->_PlayerData._BulletNum = LuaObject.getgInt("TotleBullet");*/
  return true;
}

int CGlobleData::getValue(string key)
{
  map<string, int>::iterator  iter = GlobleValue.find(key);
  if (iter != GlobleValue.end())
    return iter->second;
  return NULL;
}

/************************************************************************/
/* 映射表                                                               */
/************************************************************************/
bool CBlockMatrix::LoadConfig()
{
#ifdef _FILESYSTEM
  char* fileName = "BlockMatrix.txt";
  if (!IsFileExist(fileName, TXT_PATH))
    copyFile(fileName, TXT_PATH);
  string fullFilePath = getFilePath(fileName, TXT_PATH);
  int fileLen = strlen(fullFilePath.c_str());
  if (!fileLen)
    return false;
  char buffer[256];  
  ifstream in(fullFilePath);  
  if (! in.is_open())  
  { 
    cout << "Error opening file BlockMatrix"; 
    return false; 
  }
  int Line = 0;
  while (EOF != in.peek() )  
  { 
    in.getline (buffer,256);
    GParse GP(buffer);
    BlockMap Item;
    Item.Idx = GP.getInt();
    Item.Id = GP.getInt();
    BlockMaps.push_back(Item);
  }
  return true;
#endif;
  /*
  char* fileName = "BlockMatrix.lua";
  if (!IsFileExist(fileName, LUA_PATH))
    copyFile(fileName, LUA_PATH);
  string fullFilePath = getFilePath(fileName, LUA_PATH);
  int fileLen = strlen(fullFilePath.c_str());
  if (!fileLen)
    return false;
  if (luaL_dofile(GLuaState, fullFilePath.c_str()))
  {
    CCLOG("BlockMatrix.lua file error!");
    return false;
  }
  CLuaObject LuaObject = CLuaObject("BlockMatrix", GLuaState);
  BEGIN_LUA(LuaObject)
  {
    BlockMap Item;
    Item.Idx = LuaObject.getInt("Idx");
    Item.Id = LuaObject.getInt("Id");
    BlockMaps.push_back(Item);
  }
  END_LUA(LuaObject)

  CLuaObject LuaProObject = CLuaObject("PropertySet", GLuaState);
  BEGIN_LUA(LuaProObject)
  {
    PropertySet Property;
    Property.Idx = LuaProObject.getInt("Idx");
    Property.Id = LuaProObject.getInt("Id");
    PropertySets.push_back(Property);
  }
  END_LUA(LuaProObject)*/
  return true;
}

int CBlockMatrix::GetId( int Idx )
{
  int size = BlockMaps.size();
  for (int i = 0; i < size; i++)
  {
    if (BlockMaps[i].Idx == Idx)
      return BlockMaps[i].Id;
  }
  return -1;
}

bool CPropertySet::LoadConfig()
{
#ifdef _FILESYSTEM
  char* fileName = "PropertySet.txt";
  if (!IsFileExist(fileName, TXT_PATH))
    copyFile(fileName, TXT_PATH);
  string fullFilePath = getFilePath(fileName, TXT_PATH);
  int fileLen = strlen(fullFilePath.c_str());
  if (!fileLen)
    return false;
  char buffer[256];  
  ifstream in(fullFilePath);  
  if (! in.is_open())  
  { 
    cout << "Error opening file PropertySet"; 
    return false; 
  }
  int Line = 0;
  while (EOF != in.peek() )  
  { 
    in.getline (buffer,256);
    GParse GP(buffer);
    PropertySet Item;
    Item.Idx = GP.getInt();
    Item.Id = GP.getInt();
    PropertySets.push_back(Item);
  }
  return true;
#endif // _FILESYSTEM

  return true;
}

int CPropertySet::GetItemId( int Idx )
{
  int size = PropertySets.size();
  for (int i = 0; i < size; i++)
  {
    if (PropertySets[i].Idx == Idx)
      return PropertySets[i].Id;
  }
  return -1;
}

int CPropertySet::GetPropertySetNum()
{
  return PropertySets.size();
}

bool CShop::LoadConfig()
{

  char* fileName = "ShopItem.txt";
  if (!IsFileExist(fileName, TXT_PATH))
    copyFile(fileName, TXT_PATH);
  string fullFilePath = getFilePath(fileName, TXT_PATH);
  int fileLen = strlen(fullFilePath.c_str());
  if (!fileLen)
    return false;
  char buffer[256];  
  ifstream in(fullFilePath);  
  if (! in.is_open())  
  { 
    cout << "Error opening file ShopItem"; 
    return false; 
  }
  int Line = 0;
  while (EOF != in.peek() )  
  { 
    in.getline (buffer,256);
    GParse GP(buffer);
    ShopData Item;
    Item.Id = GP.getInt();
    Item.money = GP.getInt();
    _ShopDatas.push_back(Item);
  }
  return true;
}

const TArray<ShopData>& CShop::GetShopItemData()
{
  return _ShopDatas;
}
